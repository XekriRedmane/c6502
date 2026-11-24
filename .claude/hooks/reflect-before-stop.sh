#!/bin/bash
# reflect-before-stop.sh: SubagentStop hook.
# Keeps a subagent going once, the first time it tries to stop, with an
# instruction to run the reflect skill. The second stop carries
# "stop_hook_active": true, and the hook lets that one through, so each
# subagent reflects once. Read-only agent types, which can't write notes,
# stop at once.

input=$(cat)

# The hook already blocked this stop once.
if printf '%s' "$input" | grep -q '"stop_hook_active" *: *true'; then
    exit 0
fi

agent_type=$(printf '%s' "$input" \
    | sed -n 's/.*"agent_type" *: *"\([^"]*\)".*/\1/p' | head -1)
case "$agent_type" in
    Explore|Plan|claude-code-guide|web-fetch|statusline-setup)
        exit 0 ;;
esac

cat <<'EOF'
{"decision": "block", "reason": "Before you finish, run the reflect skill (Skill tool, skill \"reflect\") on the work you just did. Write its notes into the brain/ directory of your own working tree (your current directory) and commit them there with the Co-Authored-By line, as you commit your other work. If your working tree has no brain/ directory, don't create one: put the notes in your final message instead, under a heading \"Reflect notes\". Then end with your final report again in full, followed by one line saying what reflect saved, so that the report stays your last message."}
EOF
exit 0
