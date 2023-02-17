FROM stateoftheartio/qt6:6.4-gcc-aqt
USER root
COPY . /app
WORKDIR /app/build
ENTRYPOINT ["/usr/bin/bash", "-c", "/app/scripts/init.sh"]
