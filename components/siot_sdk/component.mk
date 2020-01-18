#
# Main Makefile. This is basically the same as a component makefile.
#
# This Makefile should, at the very least, just include $(SDK_PATH)/make/component_common.mk. By default, 
# this will take the sources in the src/ directory, compile them and link them into 
# lib(subdirectory_name).a in the build directory. This behaviour is entirely configurable,
# please read the ESP-IDF documents if you need to do this.
#
# MIIO.project by hanguangyi.
# time:02/11/2016.

COMPONENT_ADD_INCLUDEDIRS := .
COMPONENT_SRCDIRS         := .


COMPONENT_ADD_INCLUDEDIRS +=	arch \
				arch/esp32 \
				net/tcot \
				net/tcou \
				net/tcos

COMPONENT_SRCDIRS	  +=	arch \
				arch/esp32 \
				net/tcot \
				net/tcou \
				net/tcos
