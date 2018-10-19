FROM andrejlevkovitch/ubuntu_sdl_boost

RUN rm -rf /app
ADD . /app
RUN rm -rf /app/build
RUN mkdir /app/build

WORKDIR /app/build

RUN cmake -DBUILD_SHARED_LIBS=1 .. && \
    cmake --build .
RUN make test && \
    make clean && \
    rm * -rf

RUN cmake -DCMAKE_CXX_COMPILER=clang++ -DBUILD_SHARED_LIBS=1 .. && \
    cmake --build .
RUN make test && \
    make clean && \
    rm * -rf

RUN cmake -DCMAKE_CXX_COMPILER=clang++ -DBUILD_SHARED_LIBS=0 .. && \
    cmake --build .
RUN make test && \
    make clean && \
    rm * -rf

RUN cmake -DCMAKE_CXX_COMPILER=clang++ -DBUILD_SHARED_LIBS=0 -DSDL2_USE_STATIC_LIBS=ON .. && \
    cmake --build .
RUN make test && \
    make clean && \
    rm * -rf
