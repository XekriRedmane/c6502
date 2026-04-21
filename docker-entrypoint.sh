#!/bin/bash
# Docker startup script

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
