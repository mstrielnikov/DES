# lab-des
Some of my old university's lab works. Done 20.12.2019. See original paper for details `des_lab.pdf`

# Utility ussage

All input/output files and corresponded encrypted/decrypted results are kept in `resources` folder.
(I know it would be better as CLI-tool with files as arguments :laughing: But it's more just a PoC and exercise or exhibit even then practical thing)

```bash
./des
messagemessage                                                  # String to encrypt
[ 109 101 115 115 97 103 101 109 101 115 115 97 103 101 ]       # Raw message in byte-string representation
[ 205 84 60 173 86 48 217 66 8 234 67 155 53 21 225 9 ]         # Encrypted byte-string representation
[ 109 101 115 115 97 103 101 109 101 115 115 97 103 101 ]       # Decrypted byte-string for message
messagemessage                                                  # Display decrypted string to verify

```

