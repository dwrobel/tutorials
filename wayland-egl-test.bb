SUMMARY = "Wayland EGL basic test"
DESCRIPTION = "Simple application to test wayland EGL subsystem"
LICENSE = "Unlicense"
LIC_FILES_CHKSUM = "file://LICENSE;md5=0d2f730e01d19e8f457a65e092b60770"

DEPENDS = "virtual/egl virtual/libgles2 libepoxy libxkbcommon wayland wayland-egl"

S = "${WORKDIR}/git/"

SRC_URI = " \
    git://github.com/dwrobel/tutorials.git;protocol=http;branch=glesv2;rev=34a96fccc700b049c73fa34ea39939bb5e706335 \
"

inherit pkgconfig

TEST_NAME         = "wayland-egl-test"
TEST_NAME_EPOXY   = "wayland-egl-test-epoxy"

INPUT_NAME         = "wayland-egl-test-input"
INPUT_NAME_EPOXY   = "wayland-egl-test-input-epoxy"

# It's a test application, not a benchmark
TARGET_CFLAGS     += " -O0 -ggdb3"

TARGET_CC_ARCH    += "${LDFLAGS}"

do_compile () {
  cd ${S}
  ${CC} ${TARGET_CFLAGS}              -o ${TEST_NAME}       wayland-egl.c    $(pkg-config --cflags --libs       wayland-client wayland-egl glesv2 egl)
  ${CC} ${TARGET_CFLAGS} -DHAVE_EPOXY -o ${TEST_NAME_EPOXY} wayland-egl.c    $(pkg-config --cflags --libs epoxy wayland-client wayland-egl)

  ${CC} ${TARGET_CFLAGS}              -o ${INPUT_NAME}       wayland-input.c $(pkg-config --cflags --libs       wayland-client wayland-egl glesv2 egl xkbcommon)
  ${CC} ${TARGET_CFLAGS} -DHAVE_EPOXY -o ${INPUT_NAME_EPOXY} wayland-input.c $(pkg-config --cflags --libs epoxy wayland-client wayland-egl            xkbcommon)
}

do_install() {
  install -p -m 0755 -D ${S}/${TEST_NAME}         ${D}${bindir}/${TEST_NAME}
  install -p -m 0755 -D ${S}/${TEST_NAME_EPOXY}   ${D}${bindir}/${TEST_NAME_EPOXY}
  install -p -m 0755 -D ${S}/${INPUT_NAME}        ${D}${bindir}/${INPUT_NAME}
  install -p -m 0755 -D ${S}/${INPUT_NAME_EPOXY}  ${D}${bindir}/${INPUT_NAME_EPOXY}
}

FILES_${PN} += "${bindir}/*"
