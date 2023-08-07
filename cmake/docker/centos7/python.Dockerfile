FROM ortools/cmake:centos7_swig AS env
ENV PATH=/root/local/bin:$PATH
RUN wget https://www.python.org/ftp/python/3.9.6/Python-3.9.6.tgz && \
    tar -xvf Python-3.9.6.tgz && rm Python-3.9.6.tgz && \
    cd Python-3.9.6 && \
    source scl_source enable devtoolset-11 && \
    ./configure --enable-optimizations && \
    make -j 4 && \
    make altinstall
RUN python3.9 -m pip install virtualenv numpy mypy-protobuf wheel setuptools
RUN cp /usr/local/bin/python3.9 /usr/local/bin/python3
FROM env AS devel
WORKDIR /home/project
COPY . .

FROM devel AS build

RUN yum install libffi-devel -y && make install
RUN cmake -S. -Bbuild -DBUILD_PYTHON=ON -DBUILD_SAMPLES=OFF -DBUILD_EXAMPLES=OFF -DPython3_ROOT_DIR=/usr/local/bin/python3.9
RUN cmake --build build --target all -v  -j4
RUN cmake --build build --target install

FROM build AS test
RUN CTEST_OUTPUT_ON_FAILURE=1 cmake --build build --target test

FROM env AS install_env
WORKDIR /home/sample
COPY --from=build /home/project/build/python/dist/*.whl .
RUN python3.9 -m pip install *.whl

FROM install_env AS install_devel
COPY cmake/samples/python .

FROM install_devel AS install_build
RUN python3 -m compileall .

FROM install_build AS install_test
RUN python3 sample.py
