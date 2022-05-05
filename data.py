import random
import csv

names = ['Kevin','Andy','Esther','Tony', 'Elisa', 'Bella', 'Anna', 'Daniel', 'Yamamoto', 'Troye']
classes = ['food','drink', 'transportation', 'entertainment', 'camping gear']
items = [['curry rice','steak','fruit','BBQ','beef noodle', 'hotpot', 'bread', 'pizza', 'seafood', 'salad'],
        ['apple juice','bubble tea', 'milk tea', 'black tea', 'green tea', 'espresso', 'latte', 'oolong tea', 'cappuccino', 'mocha'],
        ['bus', 'taxi', 'uber', 'rental cars', 'Ubike', 'MRT', 'train', 'THSR', 'fuel expense'],
        ['hotel', 'amusement park', 'museum', 'concert', 'theater', 'gallery', 'KTV', 'zoo', 'aquarium'],
        ['tent', 'flashlight', 'sleeping bag', 'gas stove', 'ice cooler', 'fishing pole', 'hammock', 'canteen']]

random.seed(2)

data_num = 50
with open('data.csv', 'w', newline='') as csvfile:
    for _ in range(data_num): 
        name = names[random.randint(0,len(names))-1]
        cls = random.randint(0,len(classes)-1)
        classs = classes[cls]
        item = items[cls][random.randint(0,len(items[cls])-1)]
        price = random.randint(1,1000)
        date = str(random.randint(1,12))+'/'+str(random.randint(1,28))
        data_writer = csv.writer(csvfile, delimiter=',')
        data_writer.writerow([name, classs, item, price,date])
