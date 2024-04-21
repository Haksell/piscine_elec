DIRS := day*/ex* rush*

clean:
	for dir in $(DIRS); do \
		${MAKE} -C $$dir $@; \
	done