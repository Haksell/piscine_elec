DIRS := day*/ex* rush*/ex*

clean:
	for dir in $(DIRS); do \
		${MAKE} -C $$dir $@ || true; \
	done