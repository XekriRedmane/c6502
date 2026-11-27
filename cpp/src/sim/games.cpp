#include "sim/games.h"

#include <cstdlib>
#include <fstream>
#include <iterator>
#include <sstream>
#include <stdexcept>

#include "sim/harness.h"

namespace c6502::sim::games {

std::optional<std::string> env(const char* name) {
  const char* value = std::getenv(name);
  if (value == nullptr || *value == '\0') return std::nullopt;
  return std::string(value);
}

Bytes read_bytes(const std::string& path) {
  std::ifstream in(path, std::ios::binary);
  if (!in) throw std::runtime_error("can't read " + path);
  return Bytes(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

std::string read_text(const std::string& path) {
  const Bytes bytes = read_bytes(path);
  return std::string(bytes.begin(), bytes.end());
}

// ---- Zork ----

const std::vector<ZorkSession>& zork_sessions() {
  static const std::vector<ZorkSession> sessions = {
      // The first session walks into the cellar, saves, fights the
      // troll, restores, tries words that aren't in the dictionary and
      // sentences the parser rejects, and quits. Somewhere in the fight
      // the troll wins, and Zork moves the player back to the forest
      // with random numbers.
      {"walk",
       {
           "open mailbox", "read leaflet", "drop leaflet", "north", "north", "climb tree", "take egg",
           "down", "south", "east", "open window", "enter house", "take sack", "open it", "take all",
           "inventory", "west", "take lamp", "take sword", "move rug", "open trap door", "turn on lamp",
           "down", "score", "save", "south", "east", "take painting", "west", "north", "diagnose", "look",
           "examine troll", "kill troll with sword", "kill troll with sword", "kill troll with sword",
           "kill troll with sword", "kill troll with sword", "restore", "look", "xyzzy", "plugh",
           "take the frobozz", "go north, then west.", "what is a grue?", "brief", "verbose",
           "superbrief", "south", "east", "again", "wait", "wait", "score", "quit", "y",
       },
       ""},
      // The second turns the transcript on, saves at position 3 by
      // answering the prompts, fails to restore from the empty position
      // 5, restores from 3, reads the leaflet twice in one line so that
      // the screen fills and Zork stops for [MORE], turns the transcript
      // off, and restarts.
      {"transcript",
       {
           "script", "open mailbox", "take leaflet", "save", "north", "restore", "restore", "look",
           "read leaflet. read leaflet. inventory", "unscript", "restart", "y", "look", "quit", "y",
       },
       "362\r562\r362\r"},
  };
  return sessions;
}

const ZorkSession& zork_session(std::string_view name) {
  for (const ZorkSession& s : zork_sessions()) {
    if (s.name == name) return s;
  }
  throw std::invalid_argument("Zork has no session " + std::string(name));
}

namespace {

// Zork's boot loader reads 26 sectors from track 1 into $0800.
constexpr int kZorkMainFirstSector = 16;
constexpr int kZorkMainSectors = 26;

void set_up_zork(ZorkRun& run, const Bytes& dsk, const ZorkSession& session) {
  apple2::Apple2& a = run.machine;
  a.keys.assign(session.keys.begin(), session.keys.end());
  a.lines.assign(session.lines.begin(), session.lines.end());
  a.disks[{0x60, 1}] = dsk;
  a.disks[{0x60, 2}] = Bytes(dsk.size(), 0);
  a.boot_screen();
  run.cpu.memory[0x7C] = 0x60;
  run.cpu.memory[0x7F] = 16;
}

}  // namespace

Bytes zork_main(const Bytes& dsk) {
  Bytes out;
  for (int s = kZorkMainFirstSector; s < kZorkMainFirstSector + kZorkMainSectors; ++s) {
    const std::size_t at = apple2::dsk_sector(s / 16, s % 16);
    out.insert(out.end(), dsk.begin() + static_cast<std::ptrdiff_t>(at),
               dsk.begin() + static_cast<std::ptrdiff_t>(at + 256));
  }
  return out;
}

std::unique_ptr<ZorkRun> zork_original(const Bytes& dsk, const ZorkSession& session) {
  auto run = std::make_unique<ZorkRun>();
  const Bytes binary = zork_main(dsk);
  std::copy(binary.begin(), binary.end(), run->cpu.memory.begin() + 0x0800);
  run->cpu.pc = 0x0800;
  run->cpu.sp = 0xFF;
  set_up_zork(*run, dsk, session);
  return run;
}

std::unique_ptr<ZorkRun> zork_program(std::string_view exe, const Bytes& dsk, const ZorkSession& session) {
  Simulation sim = build_linked_sim(exe);
  auto run = std::make_unique<ZorkRun>();
  run->cpu = std::move(sim.cpu);
  run->hooks = std::move(sim.runtime.hooks);
  set_up_zork(*run, dsk, session);
  return run;
}

// ---- Lode Runner ----

const std::vector<LodeRunnerScript>& lode_runner_scripts() {
  // The keys are (n, key) pairs that press `key` just before the
  // program's KBD read number n. The read numbers were found by running
  // the original.
  static const std::vector<LodeRunnerScript> scripts = {
      // The attract mode. Nothing is pressed. MAIN waits a moment and
      // starts the demo, which plays its three levels until the player
      // dies on the third, and then the high scores show (GAME_MODE 2).
      {"demo", {}, 1, 2, LodeRunnerDisk::kMaster, 400'000'000},
      // A game. Space at the title starts it. The first key of a game
      // goes to the wait for a key with the player blinking, and the
      // joystick mode the image starts in lets the missing joystick
      // start the level first, so Ctrl-K (keyboard mode) at the game's
      // first read of KBD comes next. In the first life the player runs
      // right, picks up a piece of gold, and is caught at the ladder on
      // the right. In the second he runs left and digs a hole to his
      // right, which a guard falls into, and is caught later. Ctrl-A
      // ends each of the last three lives at its blinking player. The
      // score, 325, goes in the high-score table as ABC, after which the
      // high scores show until the time runs out and the splash screen
      // draws again (GAME_MODE 0).
      {"game",
       {
           {5, ' '}, {8, 0x0B}, {9, 'L'}, {300, 'J'}, {445, 'O'}, {692, 0x01}, {775, 0x01}, {858, 0x01},
           {900, 'A'}, {941, 'B'}, {982, 'C'}, {1023, 0x0D},
       },
       2, 0, LodeRunnerDisk::kGame, 200'000'000},
      // The editor. Ctrl-E at the title starts it. E and Return edit
      // level 1, where K K K moves the cursor right, 7 puts gold there, M
      // moves down, 1 puts a brick there, and Ctrl-S saves the level. K,
      // 8 (a guard), and Ctrl-Q quit without saving, so the editor
      // prints its prompt to save the level, and N declines. C 0 0 2
      // Return clears level 2. P 0 0 1 Return plays level 1. The input
      // mode is joystick again there, so the missing joystick starts the
      // level, and Ctrl-R in the game (one life left, and dead) ends it,
      // with no high score. The time after GAME OVER runs out, and the
      // splash screen draws again (GAME_MODE 0).
      {"editor",
       {
           {5, 0x05}, {26, 'E'}, {47, 0x0D}, {68, 'K'}, {89, 'K'}, {110, 'K'}, {131, '7'}, {152, 'M'},
           {173, '1'}, {194, 0x13}, {215, 'K'}, {236, '8'}, {257, 0x11}, {278, 'N'}, {299, 'C'},
           {320, '0'}, {341, '0'}, {362, '2'}, {383, 0x0D}, {404, 'P'}, {425, '0'}, {446, '0'},
           {467, '1'}, {488, 0x0D}, {521, 0x12},
       },
       3, 0, LodeRunnerDisk::kEditor, 100'000'000},
  };
  return scripts;
}

const LodeRunnerScript& lode_runner_script(std::string_view name) {
  for (const LodeRunnerScript& s : lode_runner_scripts()) {
    if (s.name == name) return s;
  }
  throw std::invalid_argument("Lode Runner has no script " + std::string(name));
}

LodeRunnerFiles LodeRunnerFiles::read(const std::string& dir) {
  LodeRunnerFiles files;
  files.image = read_bytes(dir + "/main.bin");
  files.disk = read_bytes(dir + "/lode_runner.do");
  files.routines = lode_runner::original_routines(read_text(dir + "/main.asm"));
  return files;
}

Bytes lode_runner_disk(const Bytes& master, LodeRunnerDisk disk) {
  Bytes out = master;
  const std::size_t at = apple2::dsk_sector(12, 15);
  if (disk == LodeRunnerDisk::kGame) std::fill_n(out.begin() + static_cast<std::ptrdiff_t>(at), 80, 0);
  if (disk == LodeRunnerDisk::kEditor) out.at(at + 255) = 0;
  return out;
}

std::function<bool(const lode_runner::Machine&)> mode_change(std::uint8_t before, std::uint8_t after) {
  // GAME_MODE's address, which both programs keep.
  constexpr std::uint16_t kGameMode = 0xA7;
  auto seen = std::make_shared<bool>(false);
  return [before, after, seen](const lode_runner::Machine& m) {
    const std::uint8_t mode = m.cpu.memory[kGameMode];
    if (mode == before) *seen = true;
    return *seen && mode == after;
  };
}

// ---- Ali Baba ----

const std::vector<AliBabaScript>& ali_baba_scripts() {
  // The keys are (n, key) pairs that press `key` just before the
  // program's KBD read number n, and the disk swaps (n, name) pairs that
  // put a disk in drive 1 just before KBD read n. The read numbers were
  // found by running the original, pressing each key a set number of
  // reads after the program cleared the strobe on the one before. Reads
  // by the kept disk code don't count.
  static const std::vector<AliBabaScript> scripts = {
      // The attract mode. Nothing is pressed. The title screen draws, and
      // its tune plays over and over while TUNE_ADVANCE polls the
      // keyboard between notes.
      {"attract", {}, 1500, 582113, {}},
      // A game. The two Spaces end the title tune and pick the keyboard
      // at GAME_INIT's prompt (SELECT_INPUT_MODE), and Return picks BEGIN
      // NEW GAME. Ali Baba walks up out of his home onto the dusty road
      // (I I) and right (K K K), to beside the rune at row 5, column 15,
      // without stepping on it. The J and I presses that follow leave him
      // where he is (the screen at reads 497 to 691 shows him there).
      // Return, K, and Return open SEE YOUR LOT, and M closes it. In the
      // turns that the Returns and J presses after it pass, a hostile
      // group attacks him (MOB_ATTACK, RESOLVE_ATTACK, and APPLY_DAMAGE
      // run) and kills him, and the reincarnation scene, the run's one
      // RWTS read (track $13, sector $C), plays. He's back in his home for
      // the last presses.
      {"game",
       {
           {10, ' '}, {12, ' '}, {18, 0x0D}, {49, 'I'}, {80, 'I'}, {141, 'K'}, {202, 'K'}, {263, 'K'},
           {284, ' '}, {345, 'J'}, {406, 'J'}, {467, 'J'}, {498, 'I'}, {529, 'I'}, {560, 'I'}, {591, 'I'},
           {692, ' '}, {723, 0x0D}, {754, 'K'}, {785, 0x0D}, {846, 'M'}, {877, 0x0D}, {908, 0x0D},
           {939, 'J'}, {970, 0x0D}, {1001, 0x0D}, {1032, 'J'}, {1063, 0x0D}, {1094, 0x0D}, {1125, 'J'},
           {1156, 0x0D}, {1187, 'I'}, {1218, 'I'}, {1279, 'J'}, {1340, 'J'}, {1401, 'J'}, {1462, 'J'},
           {1523, 'J'}, {1584, ' '}, {1645, ' '}, {1706, ' '},
       },
       1800, 46528, {}},
      // A saved game. After the new game, Return opens the menu at
      // DEFEND, J and I move to OTHER OPTIONS, and Return opens it; K and
      // Return open OTHER OPT, the game settings. There K Return raises
      // the game speed, K Return lowers it, I Return lowers the danger, J
      // Return raises it, J Return turns the sound off and I Return on
      // again (each sound toggle leaves MOVE selected). M and Return pick
      // SAVE GAME, and Return saves in slot 1, which is on a scenario
      // disk, so the scene that explains them plays (four Spaces). An
      // unformatted disk goes in, M and Return pick FORMAT AND USE NEW
      // SCENARIO DISK, and the game formats it (RWTS command 4), writes
      // "ALI" to track 0 sector 0 and the game's records ($4000-$59FF) to
      // tracks 1 and 2, and asks for the game disk, which goes back in
      // before the Space. Return, K, Return, and M then open and close SEE
      // YOUR LOT.
      {"save",
       {
           {10, ' '}, {12, ' '}, {18, 0x0D}, {49, 0x0D}, {80, 'J'}, {111, 'I'}, {142, 0x0D}, {173, 'K'},
           {204, 0x0D}, {235, 'K'}, {266, 0x0D}, {297, 'K'}, {328, 0x0D}, {359, 'I'}, {390, 0x0D},
           {421, 'J'}, {452, 0x0D}, {483, 'J'}, {514, 0x0D}, {545, 'I'}, {576, 0x0D}, {607, 'M'},
           {638, 0x0D}, {669, 0x0D}, {673, ' '}, {677, ' '}, {681, ' '}, {685, ' '}, {696, 'M'},
           {707, 0x0D}, {738, ' '}, {799, 0x0D}, {830, 'K'}, {861, 0x0D}, {922, 'M'},
       },
       1000, 17260, {{691, "scenario"}, {738, "game"}}},
      // A game loaded from the scenario disk that the save script writes
      // on the original. M and Return pick CONTINUE GAME NUMBER 1 at the
      // start, the scenario disk goes in before the Space that answers
      // INSERT SCENARIO DISK, the game reads its 26 sectors, and the game
      // disk goes back in before the Space that answers INSERT ALI BABA
      // DISK. Return, K, Return, and M open and close SEE YOUR LOT.
      {"load",
       {
           {10, ' '}, {12, ' '}, {18, 'M'}, {49, 0x0D}, {70, ' '}, {101, ' '}, {162, 0x0D}, {193, 'K'},
           {224, 0x0D}, {285, 'M'},
       },
       350, 9414, {{70, "scenario"}, {101, "game"}}},
      // Shopping in the forest. After the new game, Ali Baba walks onto
      // the dusty road (I I), right (K K K), up (I), right past the rune
      // (K K K K), and down (M M) through the door at row 6, column 18
      // into the forest, at row 1, column 10. Seven J presses take him
      // left to the chest of 3 ducats at row 1, column 3
      // (PICKUP_TREASURE). K K K and M take him to the weapon shop at row
      // 2, column 6, whose scene Return closes. The shop's menu shows AN
      // IRON SWORD first. Space picks BUY SOMETHING ELSE, which shows AN
      // OAK CLUB, and I and Space buy it for the 3 ducats
      // (SHOP_BUY_EFFECT_2), after which the menu shows the iron sword
      // again. I and Space try to buy it with nothing left, which moves
      // on to the club, and M and Space pick NO SALE. J J J J, six M
      // presses, and J take him to the chest of 47 ducats at row 8,
      // column 1, and I I I and K K to the armor shop at row 5, column 3.
      // The first J waits 52 reads, since with the usual 31 a tiger that
      // roams the forest catches him on the way. Return closes the scene.
      // I and Space try to buy the RITHMIL PLATE ARMOR, which he can't
      // afford, Space skips the RITHMIL CHAIN MAIL, and I and Space buy
      // the LEATHER ARMOR for 8 ducats, which gives him experience to
      // come (XP_AWARD_4, AWARD_XP). M and Space pick NO SALE. Space, I I
      // K, and Space open OTHER OPTIONS, M and Space pick DROP GOLD, Space
      // Space press # twice, K and Space Space press @ twice, and M and
      // Space drop 32 of his 39 ducats (DROP_GOLD_EXEC). The tiger is in
      // the cell above him. Space, I, and Space pick ATTACK, and I and
      // Space attack ABOVE (ATTACK_UP). By his next turn the tiger has
      // wounded him and moved into his cell. Space, I I K, Space, I, and
      // Space pick DROP ARMOR, which turns the leather armor's experience
      // into a level (FLUSH_PENDING_XP). Space, I, and Space pick ATTACK
      // again, which with the tiger in his cell goes straight to it
      // (NEXT_COLOCATED_MOB). The run ends before the tiger's turn.
      {"forest",
       {
           {10, ' '}, {12, ' '}, {18, 0x0D}, {49, 'I'}, {80, 'I'}, {111, 'K'}, {142, 'K'}, {173, 'K'},
           {204, 'I'}, {235, 'K'}, {266, 'K'}, {297, 'K'}, {328, 'K'}, {359, 'M'}, {390, 'M'}, {421, 'J'},
           {452, 'J'}, {483, 'J'}, {514, 'J'}, {545, 'J'}, {576, 'J'}, {607, 'J'}, {638, 'K'}, {669, 'K'},
           {700, 'K'}, {731, 'M'}, {762, 0x0D}, {793, ' '}, {824, 'I'}, {855, ' '}, {886, 'I'},
           {917, ' '}, {948, 'M'}, {979, ' '}, {1031, 'J'}, {1062, 'J'}, {1093, 'J'}, {1124, 'J'},
           {1155, 'M'}, {1186, 'M'}, {1217, 'M'}, {1248, 'M'}, {1279, 'M'}, {1310, 'M'}, {1341, 'J'},
           {1372, 'I'}, {1403, 'I'}, {1434, 'I'}, {1465, 'K'}, {1496, 'K'}, {1527, 0x0D}, {1558, 'I'},
           {1589, ' '}, {1620, ' '}, {1651, 'I'}, {1682, ' '}, {1713, 'M'}, {1744, ' '}, {1775, ' '},
           {1806, 'I'}, {1837, 'I'}, {1868, 'K'}, {1899, ' '}, {1930, 'M'}, {1961, ' '}, {1992, ' '},
           {2023, ' '}, {2054, 'K'}, {2085, ' '}, {2116, ' '}, {2147, 'M'}, {2178, ' '}, {2209, ' '},
           {2240, 'I'}, {2271, ' '}, {2302, 'I'}, {2333, ' '}, {2364, ' '}, {2395, 'I'}, {2426, 'I'},
           {2457, 'K'}, {2488, ' '}, {2519, 'I'}, {2550, ' '}, {2581, ' '}, {2612, 'I'}, {2643, ' '},
       },
       2674, 100842, {}},
      // A second character. At the start, M and K move to #, and Space
      // raises the slot number to 2 (SLOT_INC_ENTER). K and Space press @,
      // which lowers it to 1 again, and I and Return pick BEGIN NEW GAME.
      // Ali Baba walks onto the dusty road (I I) and right (K K K) to
      // beside the rune at row 5, column 15. Return, I, and Space pick
      // ATTACK, and K and Space attack the cell to his RIGHT. The rune is
      // an entry of the location's event list, so PLAYER_ATTACK runs and
      // takes its branch for a target that can't be hurt. Return, J I,
      // and Space open OTHER OPTIONS, and J and Space pick ADD SOMEONE,
      // whose menu shows ABOU HASSAN (CHAR_SELECT_INIT). K and Space press
      // #, which shows LUTHIEN, and K and Space press @, which shows ABOU
      // HASSAN again. M and Space pick TO THIS ROOM, M and Space TO ALI
      // BABA'S HOME, and Space ADD, and ABOU HASSAN joins
      // (INIT_CHAR_SELECT). I and Space pick MOVE. Return, J I, Space, K,
      // and Space open OTHER OPT, the game settings, and M and Space pick
      // SAVE GAME. K and Space press #, K and Space press @
      // (SLOT_INC_REENTER, SLOT_DEC_REENTER), and K and Space save in slot
      // 1. It's on a scenario disk, so the scene that explains them plays
      // (four Spaces). M and Space pick FORMAT AND USE NEW SCENARIO DISK
      // with the game disk still in the drive, so the game prints INSERT
      // SCENARIO DISK and shows the choices again
      // (SCENARIO_INSERT_PROMPT), and I and Space pick CANCEL REQUEST.
      // Return, J I, Space, K M, and Space pick RETIRE, and Ali Baba
      // retires (RETIRE_CHARACTER). The first K ends the scene, and Abou
      // Hassan walks right (K K K) onto the rune, whose scene plays (two
      // Spaces) and which hurts him (TRIGGER_SCENE_EVENT). M walks him
      // into the wall below, which stops him (SHOW_LOCKED_MSG).
      {"party",
       {
           {10, ' '}, {12, ' '}, {43, 'M'}, {74, 'K'}, {105, ' '}, {136, 'K'}, {167, ' '}, {198, 'I'},
           {229, 0x0D}, {260, 'I'}, {291, 'I'}, {322, 'K'}, {353, 'K'}, {384, 'K'}, {415, 0x0D},
           {446, 'I'}, {477, ' '}, {508, 'K'}, {539, ' '}, {570, 0x0D}, {601, 'J'}, {632, 'I'},
           {663, ' '}, {694, 'J'}, {725, ' '}, {756, 'K'}, {787, ' '}, {818, 'K'}, {849, ' '}, {880, 'M'},
           {911, ' '}, {942, 'M'}, {973, ' '}, {1004, ' '}, {1035, 'I'}, {1066, ' '}, {1097, 0x0D},
           {1128, 'J'}, {1159, 'I'}, {1190, ' '}, {1221, 'K'}, {1252, ' '}, {1283, 'M'}, {1314, ' '},
           {1345, 'K'}, {1376, ' '}, {1407, 'K'}, {1438, ' '}, {1469, 'K'}, {1500, ' '}, {1531, ' '},
           {1562, ' '}, {1593, ' '}, {1624, ' '}, {1655, 'M'}, {1686, ' '}, {1717, 'I'}, {1748, ' '},
           {1779, 0x0D}, {1810, 'J'}, {1841, 'I'}, {1872, ' '}, {1903, 'K'}, {1934, 'M'}, {1965, ' '},
           {1996, 'K'}, {2027, 'K'}, {2058, 'K'}, {2089, 'K'}, {2120, ' '}, {2151, ' '}, {2182, 'M'},
       },
       2213, 103140, {}},
  };
  return scripts;
}

const AliBabaScript& ali_baba_script(std::string_view name) {
  for (const AliBabaScript& s : ali_baba_scripts()) {
    if (s.name == name) return s;
  }
  throw std::invalid_argument("Ali Baba has no script " + std::string(name));
}

AliBabaFiles::AliBabaFiles(const std::string& root)
    : main_bin(read_bytes(root + "/output/main.bin")), disk(read_bytes(root + "/" + std::string(kAliBabaDisk))) {
  labels = ali_baba::original_labels(read_text(root + "/output/main.asm"), read_text(root + "/output/main.sym"));
  for (const auto& [addr, label] : labels) routines[addr] = label.name;
}

ali_baba::DiskImages AliBabaFiles::disks(const AliBabaScript& script) {
  if (script.name == "save") return {{"scenario", std::nullopt}};
  if (script.name == "load") return {{"scenario", scenario()}};
  return {};
}

const Bytes& AliBabaFiles::scenario() {
  if (!scenario_) {
    const std::unique_ptr<ali_baba::Machine> machine = run_original(ali_baba_script("save"));
    scenario_ = machine->medium("scenario").to_dsk();
  }
  return *scenario_;
}

std::unique_ptr<ali_baba::Machine> AliBabaFiles::original(const AliBabaScript& script) {
  return ali_baba::original_machine(main_bin, disk, script.keys, script.swaps, disks(script));
}

void AliBabaFiles::play_original(ali_baba::Machine& machine, const AliBabaScript& script) {
  const int end = script.end;
  const ali_baba::End why =
      machine.run(script.max_cycles, [end](const ali_baba::Machine& m) { return m.kbd_reads >= end; });
  if (why != ali_baba::End::kStop) {
    throw std::runtime_error("the original's run ended with " + ali_baba::end_name(why));
  }
}

std::unique_ptr<ali_baba::Machine> AliBabaFiles::run_original(const AliBabaScript& script) {
  std::unique_ptr<ali_baba::Machine> machine = original(script);
  play_original(*machine, script);
  return machine;
}

std::unique_ptr<ali_baba::Machine> ali_baba_program(AliBabaFiles& files, const binutils::elf::ElfFile& exe,
                                                    const std::string& linker_script, const AliBabaScript& script) {
  ali_baba::ProgramOptions options;
  options.clear = ali_baba::link_regions(linker_script);
  return ali_baba::program_machine(files.main_bin, exe, files.disk, script.keys, script.swaps, files.disks(script),
                                   options);
}

}  // namespace c6502::sim::games
