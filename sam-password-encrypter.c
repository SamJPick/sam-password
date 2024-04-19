#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

uint8_t streq(char *a, char *b, int len) {
	for (int i = 0; i < len; i++) {
		if (a[i] != b[i]) {
			return 0;
		}
	}
	return 1;
}

const uint8_t S_BOX[256] = {
0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 
0xd7, 0xab, 0x76, 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 
0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 
0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, 0x04, 0xc7, 0x23, 0xc3, 
0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, 0x09, 
0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 
0x2f, 0x84, 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 
0x39, 0x4a, 0x4c, 0x58, 0xcf, 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 
0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, 0x51, 0xa3, 0x40, 0x8f, 0x92, 
0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 0xcd, 0x0c, 
0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 
0x73, 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 
0xde, 0x5e, 0x0b, 0xdb, 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 
0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 
0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, 0xba, 0x78, 0x25, 
0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, 
0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 
0xc1, 0x1d, 0x9e, 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 
0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 
0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};

const uint8_t S_BOX_INVERSE[256] = {
0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 
0xf3, 0xd7, 0xfb, 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 
0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 
0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e, 0x08, 0x2e, 0xa1, 0x66, 
0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25, 0x72, 
0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 
0xb6, 0x92, 0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 
0x57, 0xa7, 0x8d, 0x9d, 0x84, 0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 
0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06, 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 
0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, 0x3a, 0x91, 
0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 
0x73, 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 
0x1c, 0x75, 0xdf, 0x6e, 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 
0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 
0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4, 0x1f, 0xdd, 0xa8, 
0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f, 
0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 
0xc9, 0x9c, 0xef, 0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 
0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61, 0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 
0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d};

uint32_t subWord(uint32_t word) {
    uint32_t result = 0;
    for (int i = 0; i < 4; i++) {
        result += S_BOX[word % 256] << (i * 8);
        word >>= 8;
    }
    return result;
}

uint32_t rotWord(uint32_t word) {
    uint8_t leftByte = word >> 24;
    return (word << 8) + leftByte;
}

uint32_t invRotWord(uint32_t word) {
	uint8_t rightByte = word % 256;
	return (word >> 8) + (rightByte << 24);
}

uint32_t rcon(uint32_t rndNum) {
    if (rndNum == 1) {
        return 0x01000000;
	}
    else if (rcon(rndNum - 1) < 0x80000000) {
        return 2 * rcon(rndNum - 1);
    }
    else {
        return ((2 * rcon(rndNum - 1)) ^ 0x11b) << 24;
    }
}

uint32_t roundWord(const uint8_t FIRST_KEY[4][4], uint8_t wordNum) {
    if (wordNum < 4) {
        return (FIRST_KEY[wordNum][0] << 24) +
               (FIRST_KEY[wordNum][1] << 16) +
               (FIRST_KEY[wordNum][2] << 8) +
               (FIRST_KEY[wordNum][3]);
    }
    else if (wordNum >= 4 && wordNum % 4 == 0) {
            return roundWord(FIRST_KEY, wordNum - 4) ^ 
                   subWord(rotWord(roundWord(FIRST_KEY, wordNum - 1))) ^ 
                   rcon(wordNum / 4);
    }
    else {
        return roundWord(FIRST_KEY, wordNum - 4) ^ 
               roundWord(FIRST_KEY, wordNum - 1);
    }
}

uint8_t gmul(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    uint8_t carry;
    for (int i = 0; i < 8; i++) {
        if (b % 2 == 1) {
            p ^= a;
        }
        b >>= 1;
        carry = a >> 7;
        a <<= 1;
        if(carry) {
            a ^= 0x1b;
        }
    }
    return p;
}

// ARGS: sam-password [r or g] password site
int main(int argc, char *argv[]) {
	uint8_t randFlag = streq(argv[1], "r", 2);
	char *password = argv[2];
	const uint8_t BLOCK_COUNT = (strlen(password) + 15) / 16;
    uint8_t FIRST_KEY[4][4];
	uint8_t initVector[4][4];
	if (!randFlag) {
        // Read keys from ~/.sam-password-keys
		char keysPath[PATH_MAX];
		sprintf(keysPath, "%s/.sam-password-keys", getenv("HOME"));
		FILE *fp = fopen(keysPath, "r");
        if (fp == NULL) {
            printf("sam-password-encrypter: ~/.sam-password-keys inaccessible\n");
            return 1;
        }
		int len = strlen(argv[3]) + 1;
		char temp[len];
		char *statusPointer = temp;
		do {
			statusPointer = fgets(temp, len, fp);
		} while (statusPointer != NULL && !streq(temp, argv[3], len));
		if (statusPointer != NULL) {
			while (fgetc(fp) != '\n') {}
			for (int i = 0; i < 16; i++) {
				char numStr[3];
				numStr[0] = fgetc(fp);
				numStr[1] = fgetc(fp);
				numStr[2] = '\0';
				FIRST_KEY[i / 4][i % 4] = strtol(numStr, NULL, 16);
			}
			while (fgetc(fp) != '\n') {}
			for (int i = 0; i < 16; i++) {
				char numStr[3];
				numStr[0] = fgetc(fp);
				numStr[1] = fgetc(fp);
				numStr[2] = '\0';
				initVector[i / 4][i % 4] = strtol(numStr, NULL, 16);
			}
			fclose(fp);
		}
		else {
			printf("sam-password-encrypter: site name not found\n");
			return 1;
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				srand(time(NULL) + rand());
				FIRST_KEY[i][j] = rand();
				srand(time(NULL) + rand());
				initVector[i][j] = rand();
			}
		}
	}
	const uint8_t FIRST_IV[4][4] = {
		{initVector[0][0], initVector[0][1], initVector[0][2], initVector[0][3]},
		{initVector[1][0], initVector[1][1], initVector[1][2], initVector[1][3]},
		{initVector[2][0], initVector[2][1], initVector[2][2], initVector[2][3]},
		{initVector[3][0], initVector[3][1], initVector[3][2], initVector[3][3]}
	};
	// Key expansion
    uint8_t roundKeys[10][4][4];
    uint8_t currentRoundKey[4][4];
    uint32_t currentRoundWord;
    for (int i = 1; i < 11; i++) {
        for (int j = 0; j < 4; j++) {
            currentRoundWord = roundWord(FIRST_KEY, (i * 4) + j);
            for (int k = 0; k < 4; k++) {
                roundKeys[i-1][j][k] = (currentRoundWord >> ((3 - k) * 8)) % 256;
            }
        }
    }
	uint8_t stateMatrix[4][4];
	uint8_t stateMatrices[BLOCK_COUNT][4][4];
	uint32_t currentWord;
	uint8_t col[4];
	uint8_t colCopy[4];
	// CBC loop
	for (int block = 0; block < BLOCK_COUNT; block++) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if ((i * 4) + j < strlen(password)) {
                    stateMatrix[i][j] = password[(i * 4) + j + (16 * block)];
                }
                else {
                    stateMatrix[i][j] = 0;
                }
            }
    	}
    	for (int i = 0; i < 4; i++) {
    		for (int j = 0; j < 4; j++) {
    			stateMatrix[i][j] ^= initVector[i][j];
    		}
    	}
    	for (int i = 0; i < 4; i++) {
    		for(int j = 0; j < 4; j++) {
    			stateMatrix[i][j] ^= FIRST_KEY[i][j];
    		}
    	}
    	// 9 main encryption rounds
    	for(int round = 0; round < 9; round++) {    
    		for (int i = 0; i < 4; i++) {
    			for (int j = 0; j < 4; j++) {
    				stateMatrix[i][j] = S_BOX[stateMatrix[i][j]];
    			}
    		}
    		for (int i = 1; i < 4; i++) {
    			currentWord = (stateMatrix[i][0] << 24) +
    			              (stateMatrix[i][1] << 16) +
    			              (stateMatrix[i][2] << 8) +
    			              (stateMatrix[i][3]);
    			for (int j = 0; j < i; j++) {
    				currentWord = rotWord(currentWord);
    			}
                stateMatrix[i][0] = currentWord >> 24;
                stateMatrix[i][1] = (currentWord >> 16) % 256;
                stateMatrix[i][2] = (currentWord >> 8) % 256;
                stateMatrix[i][3] = currentWord % 256;
    		}
    		for (int i = 0; i < 4; i++) {
    			for (int j = 0; j < 4; j++) {
    				col[j] = stateMatrix[j][i];
    			}
    			stateMatrix[0][i] = gmul(2, col[0]) ^ gmul(3, col[1]) ^ gmul(1, col[2]) ^ gmul(1, col[3]);
    			stateMatrix[1][i] = gmul(1, col[0]) ^ gmul(2, col[1]) ^ gmul(3, col[2]) ^ gmul(1, col[3]);
    			stateMatrix[2][i] = gmul(1, col[0]) ^ gmul(1, col[1]) ^ gmul(2, col[2]) ^ gmul(3, col[3]);
    			stateMatrix[3][i] = gmul(3, col[0]) ^ gmul(1, col[1]) ^ gmul(1, col[2]) ^ gmul(2, col[3]);
    		}
    		for (int i = 0; i < 4; i++) {
    			for(int j = 0; j < 4; j++) {
    				stateMatrix[i][j] ^= roundKeys[round][i][j];
    			}
    		}
    	}
    	// Final encryption round
    	for (int i = 0; i < 4; i++) {
    		for (int j = 0; j < 4; j++) {
    			stateMatrix[i][j] = S_BOX[stateMatrix[i][j]];
    		}
    	}
    	for (int i = 1; i < 4; i++) {
    		currentWord = (stateMatrix[i][0] << 24) +
    		              (stateMatrix[i][1] << 16) +
    		              (stateMatrix[i][2] << 8) +
    		              (stateMatrix[i][3]);
    		for (int j = 0; j < i; j++) {
    			currentWord = rotWord(currentWord);
    		}
            stateMatrix[i][0] = currentWord >> 24;
            stateMatrix[i][1] = (currentWord >> 16) % 256;
            stateMatrix[i][2] = (currentWord >> 8) % 256;
            stateMatrix[i][3] = currentWord % 256;
    	}
    	for (int i = 0; i < 4; i++) {
    		for(int j = 0; j < 4; j++) {
    			stateMatrix[i][j] ^= roundKeys[9][i][j];
    		}
    	}
        // Setup for next CBC iteration
    	for (int i = 0; i < 4; i++) {
    		for (int j = 0; j < 4; j++) {
    			initVector[i][j] = stateMatrix[i][j];
				stateMatrices[block][i][j] = stateMatrix[i][j];
    		}
    	}
	}
	if (randFlag) {
        // Print new keys to ~/.sam-password-keys
		char keysPath[PATH_MAX];
		sprintf(keysPath, "%s/.sam-password-keys", getenv("HOME"));
		FILE *fp = fopen(keysPath, "a");
		fprintf(fp, "%s\n", argv[3]);
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				fprintf(fp, "%02x", FIRST_KEY[i][j]);
			}
		}
		fprintf(fp, "\n");
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				fprintf(fp, "%02x", FIRST_IV[i][j]);
			}
		}
		fprintf(fp, "\n\n");
		fclose(fp);
	}
    // Output encrypted password to be piped to clipboard via xsel
	for (int i = 0; i < BLOCK_COUNT; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				if (stateMatrices[i][j][k] < 127 && stateMatrices[i][j][k] > 32) {
					printf("%c", stateMatrices[i][j][k]);
				}
				else {
					printf("%c", (stateMatrices[i][j][k] % 94) + 33);
				}
			}
		}
	}
	return 0;
}
