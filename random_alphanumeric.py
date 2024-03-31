import string
import random

# All alphanumeric characters including space
characters = string.ascii_letters + string.digits + " "

# Shuffle the characters
random_characters = list(characters)
random.shuffle(random_characters)

# Join the shuffled characters back into a string
random_string = ''.join(random_characters)

print(random_string)