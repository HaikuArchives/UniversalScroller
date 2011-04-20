export
SUBDIRS=src/filter src/preferences
VERSION=$(shell if test -z $$VERSION ; then if test -e .git ; then echo git-$(shell git branch | grep '*' | sed -e 's/^[[:space:]]*\*[[:space:]]*//' -e 's/[^a-zA-Z]/_/g')-$(shell date +'%Y%m%d') ; else echo "UNKNOWN" ; fi ; else echo $$VERSION ; fi )

NAME=UniversalScroller
URL=http://lirum.at/users/c.reuter/$(NAME)
EMAIL=c.reuter@lirum.at

all:
	for SUBDIR in $(SUBDIRS) ; do pushd $$SUBDIR && $(MAKE) all && popd ; done

clean:
	for SUBDIR in $(SUBDIRS) ; do pushd $$SUBDIR && $(MAKE) clean && popd ; done

dist: all
	rm -rf dist
	mkdir dist
	scripts/substituting_cp.sh doc/readme.txt dist
	scripts/substituting_cp.sh scripts/install.sh dist
	scripts/substituting_cp.sh scripts/uninstall.sh dist
	cp src/preferences/obj.$(BE_HOST_CPU)/Preferences dist
	cp src/filter/obj.$(BE_HOST_CPU)/UniversalScroller dist

dist-zip: dist
	rm -rf $(NAME)-$(VERSION)
	cp -a dist $(NAME)-$(VERSION)
	zip -r $(NAME)-$(VERSION).zip $(NAME)-$(VERSION)
	rm -rf $(NAME)-$(VERSION)

release:
	if test x$(VERSION) != x$$VERSION ; then echo -e "\n\n\nTry issuing:\n\n  VERSION=3.9 make release\n\n" >&2 ; exit 1 ; fi
	$(MAKE) dist-zip
