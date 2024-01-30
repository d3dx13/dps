import time

from picamera2 import Picamera2, Preview
from picamera2.encoders import H264Encoder
from picamera2.outputs import FileOutput

picam2 = Picamera2()
picam2.start_preview(Preview.QT)

preview_config = picam2.create_video_configuration(main={"size": (640, 360), "format": "BGR888"})
preview_config['raw']['size'] = (3840, 2160)
preview_config['queue'] = False

print(preview_config)
# preview_config['main']['size'] = (648, 486)
# preview_config['main']['size'] = (640, 480)

# preview_config['raw']['size'] = (9152, 6944)
# preview_config['raw']['size'] = (4624, 3472)
# preview_config['raw']['size'] = (3840, 2160)
# preview_config['raw']['size'] = (2312, 1736)
# preview_config['raw']['size'] = (1920, 1080)
# preview_config['raw']['size'] = (2592, 1944)

picam2.configure(preview_config)

picam2.start()

picam2.set_controls({"AfMode": 2})

time.sleep(5)

picam2.set_controls({"ScalerCrop": (9152 // 2 - 320, 6944 // 2 - 180, 640, 360)})
# picam2.set_controls({"ScalerCrop": (2592 // 2 - 648 // 2, 1944 // 2 - 486 // 2, 648, 486)})

time.sleep(1e9)

"""
Mode selection for 4096:3074:12:P
    SRGGB10_CSI2P,1280x720/0 - Score: 11507
    SRGGB10_CSI2P,1920x1080/0 - Score: 9506.99
    SRGGB10_CSI2P,2312x1736/0 - Score: 7246.01
    SRGGB10_CSI2P,3840x2160/0 - Score: 3506.99
    SRGGB10_CSI2P,4624x3472/0 - Score: 1233.51
    SRGGB10_CSI2P,8000x6000/0 - Score: 2707.83
    SRGGB10_CSI2P,9152x6944/0 - Score: 3274.98

0 : ov5647 [2592x1944 10-bit GBRG] (/base/axi/pcie@120000/rp1/i2c@88000/ov5647@36)
    Modes: 'SGBRG10_CSI2P' : 640x480 [58.92 fps - (16, 0)/2560x1920 crop]
                             1296x972 [43.25 fps - (0, 0)/2592x1944 crop]
                             1920x1080 [30.62 fps - (348, 434)/1928x1080 crop]
                             2592x1944 [15.63 fps - (0, 0)/2592x1944 crop]
"""
