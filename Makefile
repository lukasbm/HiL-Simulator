.PHONY: all clean cleanall checkmakefiles makefiles test cleanresults

all: checkmakefiles
	cd src && $(MAKE)

clean: checkmakefiles
	cd src && $(MAKE) clean

cleanall: checkmakefiles cleanresults
	cd src && $(MAKE) MODE=release clean
	cd src && $(MAKE) MODE=debug clean
	rm -f src/Makefile
	rm -rf out/
	cd tests && rm -rf work/
	cd analyze && rm -rf __pycache__/

cleanresults:
	rm -f results/
	cd simulations && find -type f -name '.qtenvrc' -delete
	cd simulations && find -type d -name 'results' -exec rm -rf "{}" \;
#	cd analyze && find -type d -name 'output' -exec rm -rf "{}" \;

makefiles:
	cd src && opp_makemake -f --deep

test: all
	cd tests && sh runtest.sh

checkmakefiles:
	@if [ ! -f src/Makefile ]; then \
	echo; \
	echo '======================================================================='; \
	echo 'src/Makefile does not exist. Please use "make makefiles" to generate it!'; \
	echo '======================================================================='; \
	echo; \
	exit 1; \
	fi
