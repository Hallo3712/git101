# Import the YOLO class from the ultralytics library
from ultralytics import YOLO

# ==================== Configuration ====================
# The path to your YAML dataset configuration file
# The path has been updated to point to the correct location of the YAML file
DATA_YAML_PATH = "takraw_dataset/takraw_data.yaml"

# The number of training epochs (how many times the model will see the entire dataset)
NUM_EPOCHS = 50

# The base model to start from (YOLOv8 nano model)
BASE_MODEL = "yolov8n.pt"

# The size of the images to use during training (in pixels)
IMAGE_SIZE = 640

# =======================================================
def fine_tune_yolov8_model(data_yaml_path, num_epochs, base_model, image_size):
    """
    Fine-tunes a YOLOv8 model on a custom dataset.

    Args:
        data_yaml_path (str): Path to the dataset YAML file.
        num_epochs (int): Number of training epochs.
        base_model (str): The YOLOv8 base model to use.
        image_size (int): The image size for training.
    """
    try:
        # Load the base YOLOv8 model
        model = YOLO(base_model)
        
        print(f"Starting fine-tuning with the following parameters:")
        print(f"  - Dataset YAML: {data_yaml_path}")
        print(f"  - Base Model: {base_model}")
        print(f"  - Epochs: {num_epochs}")
        print(f"  - Image Size: {image_size}")

        # Start the training process
        # The 'project' and 'name' parameters create a new directory for the results
        # 'patience' stops training early if there's no improvement, preventing overfitting
        results = model.train(
            data=data_yaml_path,
            epochs=num_epochs,
            imgsz=image_size,
            project="takraw_yolov8",
            name="fine_tuned_model",
            patience=20
        )
        print("Fine-tuning completed successfully!")
        
    except FileNotFoundError:
        print(f"Error: The file '{data_yaml_path}' was not found.")
        print("Please ensure your takraw_data.yaml file is in the correct directory.")
    except Exception as e:
        print(f"An unexpected error occurred during fine-tuning: {e}")

# Run the fine-tuning process
if __name__ == "__main__":
    fine_tune_yolov8_model(DATA_YAML_PATH, NUM_EPOCHS, BASE_MODEL, IMAGE_SIZE)
