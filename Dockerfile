FROM andrejlevkovitch/ubuntu_sdl_boost

RUN rm -rf /app
ADD . /app
RUN rm -rf /app/build
RUN mkdir /app/build

WORKDIR /app/build

RUN cmake -DBUILD_SHARED_LIBS=1 .. && \
    cmake --build .
RUN ctest test && \
    rm * -rf

RUN cmake -DCMAKE_CXX_COMPILER=clang++ -DBUILD_SHARED_LIBS=1 .. && \
    cmake --build .
RUN ctest test && \
    rm * -rf

RUN cmake -DCMAKE_CXX_COMPILER=clang++ -DBUILD_SHARED_LIBS=0 .. && \
    cmake --build .
RUN ctest test && \
    rm * -rf

RUN cmake -DCMAKE_CXX_COMPILER=clang++ -DBUILD_SHARED_LIBS=0 -DSDL2_USE_STATIC_LIBS=ON .. && \
    cmake --build .
RUN ctest test && \
    rm * -rf

RUN cmake -DCMAKE_C_COMPILER=i686-w64-mingw32-gcc -DCMAKE_CXX_COMPILER=i686-w64-mingw32-g++ .. && \
    cmake --build .
RUN ctest test && \
    rm * -rf
