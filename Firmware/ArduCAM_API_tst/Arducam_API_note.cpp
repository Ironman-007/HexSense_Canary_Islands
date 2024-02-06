void setup()
{
  SPI.begin();
  myUart.arducamUartBegin(115200);
  myUart.send_data_pack(7, "Hello Arduino UNO!");
  myCAM.begin();
  myUart.send_data_pack(8, "Mega start!");
  myCAM.registerCallBack(readBuffer, 200, stop_preivew);
}

CamStatus Arducam_Mega::begin(void)
{
    return ::begin(&cameraInfo);
}

CamStatus begin(ArducamCamera* camera)
{
    return camera->arducamCameraOp->begin(camera);
}

.begin = cameraBegin,

#define CAM_REG_SENSOR_RESET         0X07
#define CAM_SENSOR_RESET_ENABLE      (1 << 6)
#define CAM_REG_DEBUG_DEVICE_ADDRESS 0X0A

.deviceAddress = 0x78,

CamStatus cameraBegin(ArducamCamera* camera)
{
    // reset cpld and camera
    writeReg(camera, CAM_REG_SENSOR_RESET, CAM_SENSOR_RESET_ENABLE); // reset FPGA 
    waitI2cIdle(camera); // Wait I2c Idle
    cameraGetSensorConfig(camera);
    camera->verDateAndNumber[0] = readReg(camera, CAM_REG_YEAR_ID) & 0x3F; // year
    waitI2cIdle(camera);
    camera->verDateAndNumber[1] = readReg(camera, CAM_REG_MONTH_ID) & 0x0F; // month
    waitI2cIdle(camera);
    camera->verDateAndNumber[2] = readReg(camera, CAM_REG_DAY_ID) & 0x1F; // day
    waitI2cIdle(camera);
    camera->verDateAndNumber[3] = readReg(camera, CAM_REG_FPGA_VERSION_NUMBER) & 0xFF; // day
    waitI2cIdle(camera);

    writeReg(camera, CAM_REG_DEBUG_DEVICE_ADDRESS, camera->myCameraInfo.deviceAddress);
    waitI2cIdle(camera);
    return CAM_ERR_SUCCESS;
}

void waitI2cIdle(ArducamCamera* camera)
{
    camera->arducamCameraOp->waitI2cIdle(camera);
}

.waitI2cIdle = cameraWaitI2cIdle,

#define CAM_REG_SENSOR_STATE      0x44
#define CAM_REG_SENSOR_STATE_IDLE (1 << 1)

void cameraWaitI2cIdle(ArducamCamera* camera)
{
    while ((readReg(camera, CAM_REG_SENSOR_STATE) & 0X03) != CAM_REG_SENSOR_STATE_IDLE) {
        arducamDelayMs(2);
    }
}

uint8_t readReg(ArducamCamera* camera, uint8_t addr)
{
    return camera->arducamCameraOp->readReg(camera, addr);
}

uint8_t cameraReadReg(ArducamCamera* camera, uint8_t addr)
{
    uint8_t data;
    data = busRead(camera, addr & 0x7F);
    return data;
}

uint8_t busRead(ArducamCamera* camera, int address)
{
    return camera->arducamCameraOp->busRead(camera, address);
}

.busRead = cameraBusRead;

uint8_t cameraBusRead(ArducamCamera* camera, int address)
{
    uint8_t value;
    arducamSpiCsPinLow(camera->csPin);
    arducamSpiTransfer(address);
    value = arducamSpiTransfer(0x00);
    value = arducamSpiTransfer(0x00);
    arducamSpiCsPinHigh(camera->csPin);
    return value;
}

myCAM.registerCallBack(readBuffer, 200, stop_preivew);

void Arducam_Mega::registerCallBack(BUFFER_CALLBACK function, uint8_t blockSize, STOP_HANDLE handle)
{
    ::registerCallback(&cameraInfo, function, blockSize, handle);
}

void registerCallback(ArducamCamera* camera, BUFFER_CALLBACK function, uint8_t blockSize, STOP_HANDLE handle)
{
    camera->arducamCameraOp->registerCallback(camera, function, blockSize, handle);
}

.registerCallback = cameraRegisterCallback,

void cameraRegisterCallback(ArducamCamera* camera, BUFFER_CALLBACK function, uint8_t size, STOP_HANDLE handle)
{
    camera->callBackFunction = function;
    camera->blockSize        = size;
    camera->handle           = handle;
}

uint8_t uartCommandProcessing(&myCAM, commandBuff) {
    case SET_PICTURE_RESOLUTION: // Set Camera Resolution
        cameraResolution = commandBuff[1] & 0x0f;
        cameraFarmat = (commandBuff[1] & 0x70) >> 4;
        myCAM->takePicture((CAM_IMAGE_MODE)cameraResolution, (CAM_IMAGE_PIX_FMT)cameraFarmat);
        break;

    case TAKE_PICTURE:
        myCAM->takePicture((CAM_IMAGE_MODE)cameraResolution, (CAM_IMAGE_PIX_FMT)cameraFarmat);
        cameraGetPicture(myCAM);
        break;
}

CamStatus Arducam_Mega::takePicture(CAM_IMAGE_MODE mode, CAM_IMAGE_PIX_FMT pixel_format)
{
    return ::takePicture(&cameraInfo, mode, pixel_format);
}

takePicture = cameraTakePicture,

#define CAM_REG_FORMAT             0X20
#define CAM_REG_CAPTURE_RESOLUTION 0X21

#define CAM_SET_CAPTURE_MODE       (0 << 7)

CamStatus cameraTakePicture(ArducamCamera* camera, CAM_IMAGE_MODE mode, CAM_IMAGE_PIX_FMT pixel_format)
{
    if (camera->currentPixelFormat != pixel_format) {
        camera->currentPixelFormat = pixel_format;
        writeReg(camera, CAM_REG_FORMAT, pixel_format); // set the data format
        waitI2cIdle(camera);                            // Wait I2c Idle
    }

    if (camera->currentPictureMode != mode) {
        camera->currentPictureMode = mode;
        writeReg(camera, CAM_REG_CAPTURE_RESOLUTION, CAM_SET_CAPTURE_MODE | mode);
        waitI2cIdle(camera); // Wait I2c Idle
    }

    setCapture(camera);
    return CAM_ERR_SUCCESS;
}

void writeReg(ArducamCamera* camera, uint8_t addr, uint8_t val)
{
    camera->arducamCameraOp->writeReg(camera, addr, val);
}

.writeReg = cameraWriteReg,

void cameraWriteReg(ArducamCamera* camera, uint8_t addr, uint8_t val)
{
    busWrite(camera, addr | 0x80, val);
}

uint8_t busWrite(ArducamCamera* camera, int address, int value)
{
    return camera->arducamCameraOp->busWrite(camera, address, value);
}

.busWrite = cameraBusWrite,

uint8_t cameraBusWrite(ArducamCamera* camera, int address, int value)
{
    arducamSpiCsPinLow(camera->csPin);
    arducamSpiTransfer(address);
    arducamSpiTransfer(value);
    arducamSpiCsPinHigh(camera->csPin);
    arducamDelayMs(1);
    return 1;
}

setCapture(camera);
.setCapture = cameraSetCapture,

#define ARDUCHIP_TRIG 0x44 // Trigger source
#define CAP_DONE_MASK 0x04

void cameraSetCapture(ArducamCamera* camera)
{
    // flushFifo(camera);
    clearFifoFlag(camera);
    startCapture(camera);
    while (getBit(camera, ARDUCHIP_TRIG, CAP_DONE_MASK) == 0)
        ;
    camera->receivedLength = readFifoLength(camera);
    camera->totalLength    = camera->receivedLength;
    camera->burstFirstFlag = 0;
}

.getBit = cameraGetBit,

// getBit(camera, ARDUCHIP_TRIG, CAP_DONE_MASK)
uint8_t cameraGetBit(ArducamCamera* camera, uint8_t addr, uint8_t bit)
{
    uint8_t temp;
    temp = readReg(camera, addr);
    temp = temp & bit;
    return temp;
}

.clearFifoFlag = cameraClearFifoFlag,

#define ARDUCHIP_FIFO       0x04 // FIFO and I2C control
#define FIFO_CLEAR_ID_MASK  0x01
#define FIFO_START_MASK     0x02

void cameraClearFifoFlag(ArducamCamera* camera)
{
    writeReg(camera, ARDUCHIP_FIFO, FIFO_CLEAR_ID_MASK);
}

.startCapture = cameraStartCapture,
void cameraStartCapture(ArducamCamera* camera)
{
    writeReg(camera, ARDUCHIP_FIFO, FIFO_START_MASK);
}

#define FIFO_SIZE1 0x45 // Camera write FIFO size[7:0] for burst to read
#define FIFO_SIZE2 0x46 // Camera write FIFO size[15:8]
#define FIFO_SIZE3 0x47 // Camera write FIFO size[18:16]

.readFifoLength = cameraReadFifoLength,
uint32_t cameraReadFifoLength(ArducamCamera* camera)
{
    uint32_t len1, len2, len3, length = 0;
    len1   = readReg(camera, FIFO_SIZE1);
    len2   = readReg(camera, FIFO_SIZE2);
    len3   = readReg(camera, FIFO_SIZE3);
    length = ((len3 << 16) | (len2 << 8) | len1) & 0xffffff;
    return length;
}

#define READ_IMAGE_LENGTH 255

void ArducamLink::cameraGetPicture(Arducam_Mega* myCamera)
{
    ArducamCamera* cameraInstance = myCamera->getCameraInstance();
    uint8_t headAndtail[] = {0xff, 0xaa, 0x01, 0xff, 0xbb};
    uint8_t buff[READ_IMAGE_LENGTH] = {0};

    uint8_t rtLength = 0;
    uint32_t len = myCamera->getTotalLength();
    arducamUartWriteBuff(&headAndtail[0], 3);
    arducamUartWriteBuff((uint8_t*)(&len), 4);
    arducamUartWrite(((cameraInstance->currentPictureMode & 0x0f) << 4) | 0x01); // send the picture mode back
    while (myCamera->getReceivedLength()) {
        rtLength = readBuff(cameraInstance, buff, READ_IMAGE_LENGTH);
        arducamUartWriteBuff(buff, rtLength);
    }
    arducamUartWriteBuff(&headAndtail[3], 2);
}

.readBuff = cameraReadBuff,
uint32_t cameraReadBuff(ArducamCamera* camera, uint8_t* buff, uint32_t length)
{
    if (imageAvailable(camera) == 0 || (length == 0)) {
        return 0;
    }

    if (camera->receivedLength < length) {
        length = camera->receivedLength;
    }

    arducamSpiCsPinLow(camera->csPin);
    setFifoBurst(camera);
    if (camera->burstFirstFlag == 0) {
        camera->burstFirstFlag = 1;
        arducamSpiTransfer(0x00);
    }

#ifndef arducamSpiReadBlock
    for (uint32_t count = 0; count < length; count++) {
        buff[count] = arducamSpiTransfer(0x00);
    }
#else
    arducamSpiReadBlock(buff, length);
#endif
    arducamSpiCsPinHigh(camera->csPin);
    camera->receivedLength -= length;
    return length;
}

.imageAvailable = cameraImageAvailable,
uint32_t cameraImageAvailable(ArducamCamera* camera)
{
    return camera->receivedLength;
}

#define BURST_FIFO_READ     0x3C // Burst FIFO read operation
.setFifoBurst = cameraSetFifoBurst,
void cameraSetFifoBurst(ArducamCamera* camera)
{
    arducamSpiTransfer(BURST_FIFO_READ);
}

uint8_t arducamSpiTransfer(uint8_t data)
{
    return SPI.transfer(data);
}
// ================================================================
myCAM.captureThread();

void Arducam_Mega::captureThread(void)
{
    return ::captureThread(&cameraInfo);
}

void captureThread(ArducamCamera* camera)
{
    camera->arducamCameraOp->captureThread(camera);
}

.captureThread = cameraCaptureThread,

void cameraCaptureThread(ArducamCamera* camera)
{
    if (camera->previewMode) {
        uint8_t callBackLength = readBuff(camera, callBackBuff, camera->blockSize);
        if (callBackLength != FALSE) {
            camera->callBackFunction(callBackBuff, callBackLength);
        } else {
            setCapture(camera);
        }
    }
}