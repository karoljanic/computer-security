from saleae import automation
import os
import os.path
from datetime import datetime

with automation.Manager.connect(port=10430) as manager:
    device_configuration = automation.LogicDeviceConfiguration(
        enabled_digital_channels=[0, 1],
        digital_sample_rate=16_000_000,
    )

    # Record 5 seconds of data before stopping the capture
    capture_configuration = automation.CaptureConfiguration(
        capture_mode=automation.DigitalTriggerCaptureMode(trigger_type=automation.DigitalTriggerType.RISING,
                                                          trigger_channel_index=0,
                                                          trim_data_seconds=1,
                                                          after_trigger_seconds=1),

    )

    with manager.start_capture(
            device_id='A84C40E670F0BB50',
            device_configuration=device_configuration,
            capture_configuration=capture_configuration) as capture:

        capture.wait()

        # Add an analyzer to the capture
        # Note: The simulator output is not actual SPI data
        spi_analyzer = capture.add_analyzer('SPI', label=f'Test Analyzer', settings={
            'MISO': 0,
            'Clock': 1,
            'Enable': 2,
            'Bits per Transfer': '8 Bits per Transfer (Standard)'
        })

        # Store output in a timestamped directory
        output_dir = os.path.join(os.getcwd(), f'output-{datetime.now().strftime("%Y-%m-%d_%H-%M-%S")}')
        os.makedirs(output_dir)

        # Export analyzer data to a CSV file
        analyzer_export_filepath = os.path.join(output_dir, 'spi_export.csv')
        capture.export_data_table(
            filepath=analyzer_export_filepath,
            analyzers=[spi_analyzer]
        )

        # Export raw digital data to a CSV file
        capture.export_raw_data_csv(directory=output_dir, digital_channels=[0, 1])

        # Finally, save the capture to a file
        capture_filepath = os.path.join(output_dir, 'example_capture.sal')
        capture.save_capture(filepath=capture_filepath)