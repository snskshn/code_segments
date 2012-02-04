CC		= cc
CTAGS		= -Wall

all: attack_normal attack_cc attack_cc_header userver

attack_normal: attack.c
	$(CC) $(CTAGS) -DATTACK_NORMAL $< -o $@

attack_cc: attack.c
	$(CC) $(CTAGS) -DATTACK_CC $< -o $@

attack_cc_header: attack.c
	$(CC) $(CTAGS) -DATTACK_CC_HEADER $< -o $@

userver: 
	$(MAKE) -C userver-0.8.0
	mv userver-0.8.0/userver .

clean:
	rm -rf attack_normal attack_cc attack_cc_header userver options.xml userver.pid
	$(MAKE) -C userver-0.8.0 clean
