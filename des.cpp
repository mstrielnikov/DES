#include "lib/des.h"

#define OPEN_RESOURCE "resources/open.txt"
#define DECR_RESOURCE "resources/decrypted.txt"
#define ENCR_RESOURCE "resources/encrypted.txt"
#define KEY_RESOURCE "resources/key.txt"


int main(void) {

    Message OpenMsg = Message(OPEN_RESOURCE);
    cout << OpenMsg.repr_string() << endl;
    DES Cipher = DES(OpenMsg, KEY_RESOURCE);

    Message EncryptedMsg = Cipher.encrypt();
    cout << "Encrypted: " << endl;
    EncryptedMsg.print_array();
    EncryptedMsg.write_txt(ENCR_RESOURCE);

    Message DecryptedMsg = Cipher.decrypt();
    cout << "Decrypted: " << endl;
    DecryptedMsg.print_array();
    DecryptedMsg.write_txt(DECR_RESOURCE);

    cout << "As string: " << DecryptedMsg.repr_string() << endl;
    return 0;
}