This a full MP4 player developed from scratch in C, utilizing the K64 ARM CPU. The drivers are divided in a multiple layer architecture according to their functional logic:

![Driver Architecture](https://github.com/Agusgal/Micros-TPF/blob/main/docs/mp3arch%20(1).png)

The MP3 encoding algorithms are custom-made, and to organize the program flow a hierarchical finite state machine was implemented. In order to visualize songs, artists,
albums,etc, an OLED screen was used. 

![OLED Screen]([https://github.com/Agusgal/Micros-TPF/blob/main/docs/mp3arch%20(1).png](https://github.com/Agusgal/Micros-TPF/blob/main/docs/OLED_Screen.png))
