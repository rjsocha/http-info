# syntax=docker/dockerfile:1.6
FROM alpine:3 AS build-layer
RUN apk --no-cache add gcc musl-dev make upx || apk --no-cache add gcc musl-dev make
COPY src/http-info.c src/httpserver.h /build/
WORKDIR /build
RUN gcc -o http-info -s -O2 -static http-info.c && strip -x http-info
RUN which upx && upx http-info || true
WORKDIR /dist
RUN cp /build/http-info /dist/http-info

FROM scratch
COPY --from=build-layer /dist/ /
USER 50000:50000
EXPOSE 8000
ENTRYPOINT [ "/http-info" ]
