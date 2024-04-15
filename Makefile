DIRS := day*/ex*

clean:
	for dir in $(DIRS); do \
		${MAKE} -C $$dir $@; \
	done