#!/bin/bash
# no-comments-gate.sh: PreToolUse hook on Bash, and the tool that
# records a review.
#
# On branch main, it refuses a `git commit` whose staged diff touches
# code, and a `git merge` whose incoming diff touches code, until a
# /pstack:no-comments review of that diff is recorded. Commits on other
# branches pass.
#
# A review is recorded, after /pstack:no-comments has run and its fixes
# are in, with
#   bash .claude/hooks/no-comments-gate.sh --record staged   # the staged diff
#   bash .claude/hooks/no-comments-gate.sh --record <ref>    # a merge of <ref>

code_paths=('*.cpp' '*.h' '*.hpp' '*.py' '*.sh' 'CMakeLists.txt' '*.cmake')

record_file() {
    local common
    common=$(git -C "$1" rev-parse --path-format=absolute --git-common-dir) || return 1
    echo "$common/no-comments-reviewed"
}

staged_key() {
    local diff
    diff=$(git -C "$1" diff --cached -- "${code_paths[@]}")
    [ -z "$diff" ] && return 1
    echo "commit $(printf '%s' "$diff" | git -C "$1" hash-object --stdin)"
}

merge_key() {
    local sha
    sha=$(git -C "$1" rev-parse --verify --quiet "$2^{commit}") || return 2
    [ -z "$(git -C "$1" diff --name-only "HEAD...$sha" -- "${code_paths[@]}")" ] && return 1
    echo "merge $sha"
}

if [ "$1" = "--record" ]; then
    dir=$(pwd)
    if [ "$2" = "staged" ]; then
        key=$(staged_key "$dir") || { echo "no staged code to record" >&2; exit 1; }
    else
        key=$(merge_key "$dir" "$2")
        case $? in
            1) echo "$2 brings no code changes, so its merge needs no record" >&2; exit 1 ;;
            2) echo "no commit named $2" >&2; exit 1 ;;
        esac
    fi
    file=$(record_file "$dir") || exit 1
    echo "$key" >> "$file"
    echo "recorded: $key"
    exit 0
fi

deny() {
    jq -n --arg reason "$1" '{hookSpecificOutput: {hookEventName: "PreToolUse",
        permissionDecision: "deny", permissionDecisionReason: $reason}}'
    exit 0
}

if ! command -v jq > /dev/null 2>&1; then
    echo "no-comments-gate.sh: jq not found, so commits and merges on main are not checked" >&2
    exit 0
fi

input=$(cat)
command=$(printf '%s' "$input" | jq -r '.tool_input.command // empty')
cwd=$(printf '%s' "$input" | jq -r '.cwd // empty')
[ -z "$cwd" ] && cwd=$CLAUDE_PROJECT_DIR

action=$(printf '%s' "$command" | grep -oE '\bgit( -C [^ ]+)? (commit|merge)\b' | head -1)
[ -z "$action" ] && exit 0

command_dir() {
    local leading_cd git_dash_c
    git_dash_c=$(printf '%s' "$action" | sed -nE 's/^git -C ([^ ]+) .*/\1/p')
    leading_cd=$(printf '%s' "$command" | sed -nE 's/^[[:space:]]*cd[[:space:]]+("([^"]+)"|([^ ;&]+)).*/\2\3/p')
    if [ -n "$git_dash_c" ]; then
        echo "${git_dash_c//\"/}"
    elif [ -n "$leading_cd" ]; then
        echo "$leading_cd"
    else
        echo "$cwd"
    fi
}

dir=$(command_dir)

[ "$(git -C "$dir" branch --show-current 2>/dev/null)" = "main" ] || exit 0
file=$(record_file "$dir") || exit 0

case $action in
    *commit)
        if printf '%s' "$command" | grep -qE '\bgit( -C [^ ]+)? (add|rm|mv)\b'; then
            deny "no-comments gate: stage the changes in a command of their own before the commit on main, so that the gate checks the diff the commit takes."
        fi
        if printf '%s' "$command" | grep -qE '\bgit( -C [^ ]+)? commit\b[^;&|]* (-a|--all)\b'; then
            deny "no-comments gate: commit on main from the staged changes, without -a, so that the gate checks the diff the commit takes."
        fi
        key=$(staged_key "$dir") || exit 0
        grep -qxF "$key" "$file" 2>/dev/null && exit 0
        deny "no-comments gate: the staged diff touches code and has no recorded /pstack:no-comments review. Run /pstack:no-comments on \`git diff --cached\`, apply its accepted fixes, stage them, then run \`bash .claude/hooks/no-comments-gate.sh --record staged\` and commit again."
        ;;
    *merge)
        ref=$(printf '%s' "$command" | sed -nE 's/.*\bgit( -C [^ ]+)? merge\b(( +-[^ ;&|]+)*) +([^- ;&|][^ ;&|]*).*/\4/p')
        [ -z "$ref" ] && exit 0
        key=$(merge_key "$dir" "$ref")
        case $? in
            1|2) exit 0 ;;
        esac
        grep -qxF "$key" "$file" 2>/dev/null && exit 0
        deny "no-comments gate: merging $ref into main brings code with no recorded /pstack:no-comments review. Run /pstack:no-comments on \`git diff main...$ref\`, commit its accepted fixes on $ref, then run \`bash .claude/hooks/no-comments-gate.sh --record $ref\` and merge again."
        ;;
esac
exit 0
