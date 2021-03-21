import time

# import Adafruit IO REST client.
from Adafruit_IO import Client, Feed, RequestError

# Set to your Adafruit IO key.
# Remember, your key is a secret,
# so make sure not to publish it when you publish this code!
ADAFRUIT_IO_KEY = 'AddYourKeyHere'

# Set to your Adafruit IO username.
# (go to https://accounts.adafruit.com to find your username)
ADAFRUIT_IO_USERNAME = 'AddYurUserNameHere'

# Create an instance of the REST client.
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

try: # if we have a 'whiteplayer' feed
    whiteplayermove = aio.feeds('whiteplayermove')
except RequestError: # create a text-test feed
    feed = Feed(name="whiteplayermove")
    whiteplayermove = aio.create_feed(feed)

try: # if we have a 'boardb' feed
    blackplayermove = aio.feeds('blackplayermove')
except RequestError: # create a digital feed
    feed = Feed(name="blackplayermove")
    whiteplayermove = aio.create_feed(feed)

#----------------------
sendOrReceive = "neither"
previousData = "ready"
colourChoice = "neither"

while True:
    try:
        sendOrReceive = input()
    except EOFError:
        continue
    if sendOrReceive == "send":
        colourChoice = input()
        if colourChoice == "cwhite":
            chesspiecemoved = input()
            print('White piece moved -> ', chesspiecemoved, ' succesfully.')
            aio.send(whiteplayermove.key, chesspiecemoved)
            #break
        elif colourChoice == "cblack":
            chesspiecemoved = input()
            print('Black piece moved -> ', chesspiecemoved, ' succesfully.')
            aio.send(blackplayermove.key, chesspiecemoved)
            #break
    elif sendOrReceive == "receive":
        while True:
            if colourChoice == "cwhite":
                data = aio.receive(blackplayermove.key)
                data = data.value.strip().lower()
            elif colourChoice == "cblack":
                data = aio.receive(whiteplayermove.key)
                data = data.value.strip().lower()
            if previousData != data:
                previousData = data
                print(data + '\n')
                break
            time.sleep(3)

    # avoid timeout from adafruit io
    time.sleep(2)
