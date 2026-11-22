from ultralytics import YOLO
from huggingface_hub import hf_hub_download
import sys

sys.path.append("..")
from start_capture_script import play_demo

weights = hf_hub_download(
    "leeyunjai/yolo11-face-emotion-fer2013-cls", "feryolo-11x-64.pt"
)
model = YOLO(weights)
play_demo(model)
# model.predict(source=source, save=True)
