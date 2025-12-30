# outerwear
## options
* hanger
* toilet seat
## current selection
* hanger: 
    * strengths: small, no install
    * weakness: physical stability, cleaning
## needs
* allows pressure sensing
* clean easily, brush ideally
* non-invasive
* easy to place and remove
* water-tight
* reasonably priced
* vantage point on bowl

# illumination
## options
* white light LED 
* 500 nm LED with 520-550 emission filter for stool and 600-700 filter for blood
## current selection
* white light LED
    * strengths: cheap, easy hardware, lowest power, presence of training images
    * weakness: detecting blood in edge cases; discrimination of blood and stool
## needs
* bright, stable light source

# network
## options
* toilet device broadcast wifi network, serves images to front end 
* separate server collects images from toilet device client, front end accesses server
## current selection 
* separate server
    * strengths: storage and power, easier to secure, supports multiple users, easier to send data to iPoop central
    * weakness: additional expense, setup overhead, troubleshooting
## needs
* secure
* store data indefinitely
* support analyses of data
* passive data logging and access 

# stool classification algorithm
## options
* MobileNetV2 
* MCUNetV2
## articles
* https://proceedings.mlr.press/v149/zhou21a/zhou21a.pdf
* https://openaccess.thecvf.com/content/CVPR2022W/CVPM/papers/Zhang_Human_Stools_Classification_for_Gastrointestinal_Health_Based_on_an_Improved_CVPRW_2022_paper.pdf
* https://pmc.ncbi.nlm.nih.gov/articles/PMC11350077/

# electronics
## boards
* Seeed Studio XIAO ESP32S3 Sense
    * 102010635-FA
    * OV2640 camera sensor
    * https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/
    * https://mjrovai.github.io/XIAO_Big_Power_Small_Board-ebook/
    * camera pins here: https://github.com/limengdu/SeeedStudio-XIAO-ESP32S3-Sense-camera/blob/main/take_photos/camera_pins.h#L179-L198
    * core: 
        * ESP32-S3R8
        * Xtensa LX7 dual-core, 32-bit processor up to 240 MHz
* Ardiuno feather huzzah

# system
## webserver
* plugged in
* receives logs from monitor
* serves logs to personal device app
* runs classification models? 
## monitor
* on battery
* deep sleep during inactivity
## personal device
* checks for new data from webserver
* pulls data onto device
* runs classification models? 