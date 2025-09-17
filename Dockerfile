# Stage 1: Build the application
FROM debian:stable-slim AS builder

# Install build dependencies, including libasio-dev
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libssl-dev \
    libboost-all-dev \
    libz-dev \
    git \
    wget \
    unzip \
    libasio-dev \
    && rm -rf /var/lib/apt/lists/*

# Download and install Crow.
WORKDIR /
RUN git clone https://github.com/CrowCpp/Crow.git && \
    cd Crow && \
    mkdir build && cd build && \
    cmake .. && make install

# Copy source code and build it
WORKDIR /app
COPY . .
RUN mkdir -p /app/bin # Create the bin directory
RUN cmake . && \
    make

# Stage 2: Create the final, lightweight image
FROM debian:stable-slim

# Copy the built executable from the builder stage
COPY --from=builder /app/maze_game /usr/local/bin/maze_game

# Copy the static files, ensuring the directory structure is preserved
COPY --from=builder /app/static /usr/local/bin/static

# Expose the port the application runs on
EXPOSE 8080

# Run the application
CMD ["/usr/local/bin/maze_game"]