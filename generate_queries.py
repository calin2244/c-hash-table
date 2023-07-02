import random
import string
import random

def generate_dataset(size, collision_rate, key_len, val_len):
    dataset = []
    keys = set()

    for _ in range(size):
        key = ''.join(random.choices(string.ascii_letters, k=random.randint(3, key_len)))
        value = ''.join(random.choices(string.ascii_letters, k=random.randint(3, val_len)))
        dataset.append((key, value))

        if random.random() < collision_rate:
            # Generate a colliding key
            colliding_key = key
            while colliding_key in keys:
                colliding_key = ''.join(random.choices(string.ascii_letters, k=5))
            
            # Add the colliding key-value pair
            dataset.append((colliding_key, value))
        
        keys.add(key)

    return dataset

# Generate a dataset of size 500k with a collision rate of 0.4 (10% chance of collision)
# Allocating around 78 million bytes
dataset = generate_dataset(2000, 0.2, 12, 15)

# Write the dataset to a file
with open('./Unit-Testing/Data(Input)/entries.txt', 'w') as file:
    for key, value in dataset:
        file.write(f"{key} {value}\n")