# ESP32 based TM1628 clock
<a href="https://www.buymeacoffee.com/Ua0JwY9" target="_blank"><img src="https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png" alt="Buy Me A Coffee" style="height: 41px !important;width: 174px !important;box-shadow: 0px 3px 2px 0px rgba(190, 190, 190, 0.5) !important;-webkit-box-shadow: 0px 3px 2px 0px rgba(190, 190, 190, 0.5) !important;" ></a>

Long time ago, I bought "wooden" clock from [Aliexpress](https://www.aliexpress.com/item/32514925972.html). I thought it was good purchase, until I noticed it tends to run too fast in a week or two. I am too lazy to reset correct time each few weeks, so I was on my way to trash bin with it. But then I gave it second thoughts. What if it has some sort of TM16xx chip inside? Then I could plug it to ESP board and have NTP to update correct time periodically. To my success it had TM1628 chip and what a convience - 5-pin header! After some googling, some trial and error I managed to light up segments. From there - easy walk in a park. I've found BlockThor's arduino library for TM1628 chip, few corrections, few dozens lines of coding and bingo! Clock shows correct time. Also, it has two smaller segments, which are dedicated to temperature and humidity display. Since I don't have any sensors on my ESP, I've decided to use MQTT to read temperature and humidity. Feel free to edit / reuse code as you wish.

# Schematics
| CLOCK | ESP32 | DESC |
|--|--|--|
| 1 | GPIO19 | DIO
| 2 | GPIO18 | CLK
| 3 | GPIO5 | STB
| 4 | GND | GND
| 5 | 3V3 | VCC

*Note: PIN1 on the clock is furthest from the opening hole.

I've reused original barrel-type power plug. Just keep in mind, that you need to scratch/remove traces to not provide power to original board. If your ESP keeps rebooting, it might be too long power cable. I had to shorten mine to 30-40cm. ESP32 convienently fit on the back of original board with some hot glue.

# Pictures
![Front view. Dot on the top left shows WiFi status (blink - connecting, solid - connected)](https://github.com/algirdasc/esp32-tm1628-clock/blob/main/images/PXL_20210718_091041986.jpg?raw=true)
![Back view - everything left original](https://raw.githubusercontent.com/algirdasc/esp32-tm1628-clock/2ef13ceda851d19b989a2aa9e025265446dd1471/images/PXL_20210718_091051398.jpg)
![Internals](https://github.com/algirdasc/esp32-tm1628-clock/blob/main/images/PXL_20210718_090950436.jpg?raw=true)

[Youtube DEMO](https://youtu.be/d8kvp0go3O0)

# Credits
BlockThor - https://github.com/BlockThor/TM1628
