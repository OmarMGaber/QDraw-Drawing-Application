# syntax=docker/dockerfile:1

ARG QT_VERSION=6.5.3

FROM debian:bookworm-slim AS builder

ARG QT_VERSION
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    cmake \
    g++ \
    git \
    libdbus-1-dev \
    libegl-dev \
    libfontconfig-dev \
    libfreetype6-dev \
    libgl1-mesa-dev \
    libglib2.0-0 \
    libx11-xcb1 \
    libxcb-cursor0 \
    libxkbcommon-x11-dev \
    mesa-common-dev \
    ninja-build \
    python3-pip \
    && pip3 install --break-system-packages aqtinstall \
    && rm -rf /var/lib/apt/lists/*

RUN aqt install-qt linux desktop "${QT_VERSION}" gcc_64 --outputdir /opt/Qt

ENV PATH="/opt/Qt/${QT_VERSION}/gcc_64/bin:${PATH}" \
    CMAKE_PREFIX_PATH="/opt/Qt/${QT_VERSION}/gcc_64"

WORKDIR /src
COPY . .
RUN cmake -S . -B build -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/opt/qdraw \
    && cmake --build build \
    && cmake --install build

FROM debian:bookworm-slim AS runtime

ARG QT_VERSION
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    libdbus-1-3 \
    libegl1 \
    libfontconfig1 \
    libfreetype6 \
    libgssapi-krb5-2 \
    libgl1 \
    libglib2.0-0 \
    libgles2 \
    libglx0 \
    libopengl0 \
    libx11-6 \
    libx11-xcb1 \
    libxcb-cursor0 \
    libxcb-icccm4 \
    libxcb-image0 \
    libxcb-keysyms1 \
    libxcb-randr0 \
    libxcb-render-util0 \
    libxcb-render0 \
    libxcb-shape0 \
    libxcb-shm0 \
    libxcb-sync1 \
    libxcb-util1 \
    libxcb-xfixes0 \
    libxcb-xkb1 \
    libxcb1 \
    libxkbcommon-x11-0 \
    libxkbcommon0 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=builder /opt/Qt/${QT_VERSION}/gcc_64/lib /opt/Qt/${QT_VERSION}/gcc_64/lib
COPY --from=builder /opt/Qt/${QT_VERSION}/gcc_64/plugins /opt/Qt/${QT_VERSION}/gcc_64/plugins
COPY --from=builder /opt/Qt/${QT_VERSION}/gcc_64/qml /opt/Qt/${QT_VERSION}/gcc_64/qml
COPY --from=builder /opt/qdraw /opt/qdraw

ENV LD_LIBRARY_PATH="/opt/Qt/${QT_VERSION}/gcc_64/lib" \
    QT_PLUGIN_PATH="/opt/Qt/${QT_VERSION}/gcc_64/plugins" \
    QML2_IMPORT_PATH="/opt/Qt/${QT_VERSION}/gcc_64/qml" \
    PATH="/opt/qdraw/bin:/opt/Qt/${QT_VERSION}/gcc_64/bin:${PATH}" \
    QT_QPA_PLATFORM=xcb

WORKDIR /opt/qdraw
CMD ["appQDraw"]
