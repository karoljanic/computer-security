all : key-gen.cpp rc4-encrypt.cpp rc4-decrypt.cpp check-keys.cpp bank-account-gen.cpp bank-attack.cpp
	g++ -std=c++17 -o key-gen key-gen.cpp -O3
	g++ -std=c++17 -o rc4-encrypt rc4-encrypt.cpp -O3
	g++ -std=c++17 -o rc4-decrypt rc4-decrypt.cpp -O3
	g++ -std=c++17 -o check-keys check-keys.cpp -O3
	g++ -std=c++17 -o bank-account-gen bank-account-gen.cpp -O3
	g++ -std=c++17 -o bank-attack bank-attack.cpp -O3

key : key-gen.cpp
	g++ -std=c++17 -o key-gen key-gen.cpp -O3

rc4 : rc4-encrypt.cpp rc4-decrypt.cpp
	g++ -std=c++17 -o rc4-encrypt rc4-encrypt.cpp -O3
	g++ -std=c++17 -o rc4-decrypt rc4-decrypt.cpp -O3

check : check-keys.cpp
	g++ -std=c++17 -o check-keys check-keys.cpp -O3

bank : bank-account-gen.cpp bank-attack.cpp
	g++ -std=c++17 -o bank-account-gen bank-account-gen.cpp -O3
	g++ -std=c++17 -o bank-attack bank-attack.cpp -O3

clean :
	rm key-gen rc4-encrypt rc4-decrypt check-keys bank-account-gen bank-attack
