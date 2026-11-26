FROM node:22-bookworm

# System packages: git, pdflatex (texlive), build tools for dasm and
# the compiler, and CMake
RUN apt-get update && apt-get install -y --no-install-recommends \
    git \
    build-essential \
    cmake \
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

# Install uv (Python package manager)
COPY --from=ghcr.io/astral-sh/uv:latest /uv /uvx /usr/local/bin/

# Build the compiler, c6502cpp (cpp/README.md), from the sources the
# image is built from. compile.py, nm.py, and objdump.py find it through
# C6502_CPP, so the project mounted at /project/c6502 runs it without a
# build of its own. A change to cpp/ in the mounted project takes a
# rebuild of the image, or a build in the project with C6502_CPP set to
# its cpp/build/bin/c6502cpp.
COPY cpp/CMakeLists.txt /opt/c6502/cpp/CMakeLists.txt
COPY cpp/src /opt/c6502/cpp/src
COPY cpp/generated /opt/c6502/cpp/generated
COPY cpp/tests /opt/c6502/cpp/tests
COPY cpp/third_party /opt/c6502/cpp/third_party
RUN cmake -S /opt/c6502/cpp -B /opt/c6502/cpp/build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build /opt/c6502/cpp/build --target c6502cpp --parallel
ENV C6502_CPP=/opt/c6502/cpp/build/bin/c6502cpp

# Create non-root user for Claude Code --dangerously-skip-permissions
RUN useradd -m -s /bin/bash robertbaruch \
    && mkdir -p /home/robertbaruch/.claude \
    && chown -R robertbaruch:robertbaruch /home/robertbaruch/.claude \
    && echo 'alias yolo="claude --dangerously-skip-permissions"' >> /home/robertbaruch/.bashrc

COPY docker-entrypoint.sh /usr/local/bin/docker-entrypoint.sh
RUN chmod +x /usr/local/bin/docker-entrypoint.sh

USER robertbaruch

# Point npm's global prefix at robertbaruch's home so the user can upgrade
# claude-code at runtime (npm install -g @anthropic-ai/claude-code) without
# hitting EACCES on /usr/local/lib/node_modules.
ENV NPM_CONFIG_PREFIX=/home/robertbaruch/.npm-global
ENV PATH=/home/robertbaruch/.npm-global/bin:$PATH

# Install Claude Code CLI. ADD of the npm registry's "latest" metadata
# invalidates this layer whenever a new version of claude-code is published,
# so the install below always picks up the newest release.
ADD https://registry.npmjs.org/@anthropic-ai/claude-code/latest /tmp/claude-code-latest.json
RUN mkdir -p "$NPM_CONFIG_PREFIX" \
    && npm install -g @anthropic-ai/claude-code

# Trust the bind-mounted project repo so git works inside the container
# (otherwise host/container UID mismatch trips git's dubious-ownership check).
RUN git config --global --add safe.directory /project/c6502

WORKDIR /project

ENTRYPOINT ["docker-entrypoint.sh"]
CMD ["bash"]
