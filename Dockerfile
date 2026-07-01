# 1. Use a lightweight, modern Linux environment as the base
FROM ubuntu:24.04

# 2. Prevent interactive prompts during installation
ENV DEBIAN_FRONTEND=noninteractive

# 3. Install the C++ compiler (g++) and essential build tools
RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    make \
    libasio-dev \
    && rm -rf /var/lib/apt/lists/*

# 4. Set the working directory inside the virtual container
WORKDIR /app

# 5. Copy all your local project files into the container
COPY . .

# 6. Compile your C++ code with optimization flags (-O3)
# Note: Ensure you include the pthread flag since Crow runs multithreaded
RUN g++ -O3 main.cpp -o chess_engine -lpthread

# 7. Inform the cloud platform which port your web server listens on
EXPOSE 18080

# 8. The command that starts your server when the container launches
CMD ["./chess_engine"]
