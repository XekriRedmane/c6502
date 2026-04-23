#!/bin/bash
# Docker startup script

git config --global user.name "Xekri Redmane"
git config --global user.email "robert.c.baruch@gmail.com"

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

exec "$@"
