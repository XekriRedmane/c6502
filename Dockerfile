FROM node:22-bookworm

# System packages: git, pdflatex (texlive), build tools for dasm
RUN apt-get update && apt-get install -y --no-install-recommends \
    git \
    build-essential \
    python3 \
    python3-pip \
    python3-venv \
    wget \
    unzip \
    curl \
    jq \
    && rm -rf /var/lib/apt/lists/*

# Install GitHub CLI
RUN curl -fsSL https://cli.github.com/packages/githubcli-archive-keyring.gpg | dd of=/usr/share/keyrings/githubcli-archive-keyring.gpg \
    && echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/githubcli-archive-keyring.gpg] https://cli.github.com/packages stable main" > /etc/apt/sources.list.d/github-cli.list \
    && apt-get update && apt-get install -y gh \
    && rm -rf /var/lib/apt/lists/*

# Build dasm from source
RUN git clone https://github.com/dasm-assembler/dasm.git /tmp/dasm \
    && cd /tmp/dasm \
    && make \
    && cp /tmp/dasm/bin/dasm /usr/local/bin/dasm \
    && rm -rf /tmp/dasm

# Install Claude Code CLI
RUN npm install -g @anthropic-ai/claude-code

# Install uv (Python package manager)
COPY --from=ghcr.io/astral-sh/uv:latest /uv /uvx /usr/local/bin/

# Install pcpp (C preprocessor) from source
RUN UV_TOOL_DIR=/opt/uv-tools UV_TOOL_BIN_DIR=/usr/local/bin \
    uv tool install git+https://github.com/ned14/pcpp.git

# Create non-root user for Claude Code --dangerously-skip-permissions
RUN useradd -m -s /bin/bash robertbaruch \
    && mkdir -p /home/robertbaruch/.claude \
    && chown -R robertbaruch:robertbaruch /home/robertbaruch/.claude \
    && echo 'alias yolo="claude --dangerously-skip-permissions"' >> /home/robertbaruch/.bashrc

COPY docker-entrypoint.sh /usr/local/bin/docker-entrypoint.sh
RUN chmod +x /usr/local/bin/docker-entrypoint.sh

USER robertbaruch

# Trust the bind-mounted project repo so git works inside the container
# (otherwise host/container UID mismatch trips git's dubious-ownership check).
RUN git config --global --add safe.directory /project/c6502

WORKDIR /project

ENTRYPOINT ["docker-entrypoint.sh"]
CMD ["bash"]
