import random
import string

with open("midmidbigfile.txt", "w") as f:
    for _ in range(6_250_000):  # 100 millió karakter
        f.write(random.choice(string.ascii_letters + string.digits + " "))