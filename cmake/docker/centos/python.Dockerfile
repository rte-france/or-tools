FROM ortools/cmake:centos_swig AS env
ENV PATH=/root/.local/bin:$PATH
RUN dnf -y update \
&& dnf -y install python39-devel python39-numpy \
&& dnf clean all \
&& rm -rf /var/cache/dnf

FROM env AS devel
WORKDIR /home/project
COPY . .

FROM devel AS build
RUN cmake -S. -Bbuild -DBUILD_PYTHON=ON -DBUILD_SAMPLES=OFF -DBUILD_EXAMPLES=OFF
RUN cmake --build build --target all -v -j8
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
RUN python3.9 -m compileall .

FROM install_build AS install_test
RUN python3 sample.py
