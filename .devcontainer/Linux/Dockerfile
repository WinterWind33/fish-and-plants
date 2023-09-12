FROM gcc:13

# Install CMake and git
RUN apt-get update && apt-get install -y cmake git

# Install clang tidy
RUN apt-get install -y clang-tidy

# Install Microsoft.GSL
RUN git clone https://github.com/microsoft/GSL.git /repos/GSL && \
    cd /repos/GSL && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make && \
    make install

# Install Catch2
RUN git clone  https://github.com/catchorg/Catch2.git /repos/Catch2 && \
    cd /repos/Catch2 && \
    mkdir build && \
    cmake -Bbuild -H. -DBUILD_TESTING=OFF && \
    cmake --build build/ --target install
