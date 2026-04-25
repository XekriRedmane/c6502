#!/bin/bash
# Docker startup script

git config --global user.name "Xekri Redmane"
git config --global user.email "robert.c.baruch@gmail.com"

# Redirect Claude state to the host-bind-mounted directory.
# Mounting the directory (not the .json file directly) preserves atomic-rename
# semantics — Claude Code writes .claude.json.tmp + rename, which only works
# when the rename target lives in a real directory inode.
HOST_STATE=/host-state
mkdir -p "$HOST_STATE/.claude"
[ -L "$HOME/.claude" ] || rm -rf "$HOME/.claude"
ln -sfn "$HOST_STATE/.claude" "$HOME/.claude"
ln -sfn "$HOST_STATE/.claude.json" "$HOME/.claude.json"

CLAUDE_JSON="$HOME/.claude.json"
PROJECT_PATH="/project/c6502"

python3 -c "
import json, os

path = '$CLAUDE_JSON'
project = '$PROJECT_PATH'

if os.path.exists(path):
    with open(path) as f:
        d = json.load(f)
else:
    d = {}

projects = d.setdefault('projects', {})
proj = projects.setdefault(project, {})
mcp = proj.get('mcpServers', {})
"

if [ -d /mnt/host-ssh ]; then
    mkdir -p "$HOME/.ssh"
    chmod 700 "$HOME/.ssh"
    cp /mnt/host-ssh/id_ed25519     "$HOME/.ssh/" 2>/dev/null && chmod 600 "$HOME/.ssh/id_ed25519"
    cp /mnt/host-ssh/id_ed25519.pub "$HOME/.ssh/" 2>/dev/null && chmod 644 "$HOME/.ssh/id_ed25519.pub"
    cp /mnt/host-ssh/known_hosts    "$HOME/.ssh/" 2>/dev/null && chmod 644 "$HOME/.ssh/known_hosts"
    ssh-keyscan -t ed25519,rsa github.com >> "$HOME/.ssh/known_hosts" 2>/dev/null
    sort -u "$HOME/.ssh/known_hosts" -o "$HOME/.ssh/known_hosts"
fi

exec "$@"
