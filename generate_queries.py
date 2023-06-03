import random
import string

def generate_dataset(size, collision_rate):
    dataset = []
    keys = set()

    for _ in range(size):
        key = ''.join(random.choices(string.ascii_letters, k=5))
        value = ''.join(random.choices(string.ascii_letters, k=5))
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

# Generate a dataset of size 4500 with a collision rate of 0.1 (10% chance of collision)
dataset = generate_dataset(10, 0)

# Write the dataset to a file
with open('entries.txt', 'w') as file:
    for key, value in dataset:
        file.write(f"{key} {value}\n")