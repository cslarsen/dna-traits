.PHONY:

all: dnatraits py-dnatraits

check: all
	$(MAKE) -C dnatraits check
	$(MAKE) -C py-dnatraits check

bench: all
	$(MAKE) -C py-dnatraits bench

dnatraits: .PHONY
	$(MAKE) -C $@

py-dnatraits: dnatraits
	$(MAKE) -C $@

clean:
	$(MAKE) -C dnatraits clean
	$(MAKE) -C py-dnatraits clean
