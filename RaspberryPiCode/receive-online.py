"""
'digital_out.py'
===================================
Example of turning on and off a LED
from the Adafruit IO Python Client

Author(s): Brent Rubell, Todd Treece
"""
# Import standard python modules
import time

# import Adafruit Blinka
#import digitalio

# import Adafruit IO REST client.
from Adafruit_IO import Client, Feed, RequestError

# Set to your Adafruit IO key.
# Remember, your key is a secret,
# so make sure not to publish it when you publish this code!
ADAFRUIT_IO_KEY = 'aio_FEPe45GB6EpB9Q4y7qKlpN9UOf2C'

# Set to your Adafruit IO username.
# (go to https://accounts.adafruit.com to find your username)
ADAFRUIT_IO_USERNAME = 'DIYMlewis'

# Create an instance of the REST client.
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

try: # if we have a 'digital' feed
    boardbmove = aio.feeds('boardbmove')
except RequestError: # create a digital feed
    feed = Feed(name="boardbmove")
    boardbmove = aio.create_feed(feed)
    
previousData = "nothing"

while True:
    data = aio.receive(boardbmove.key)
    if previousData != data:
        print(data.value, '\n')
        previousData = data
    time.sleep(3)
