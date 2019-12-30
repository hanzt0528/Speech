
import logging
from os import path
from os import environ
import warnings

logging.basicConfig(level=logging.CRITICAL)

environ["MEDLEYDB_PATH"]

if "MEDLEYDB_PATH" in environ and path.exists(environ["MEDLEYDB_PATH"]):
    MEDLEYDB_PATH = environ["MEDLEYDB_PATH"]
    print(MEDLEYDB_PATH)
    AUDIO_AVAILABLE = True
elif "MEDLEYDB_PATH" not in environ:
    warnings.warn(
        "The environment variable MEDLEYDB_PATH is not set. "
        "As a result, any part of the code that requires the audio won't work. "
        "If you don't need to access the audio, disregard this warning. "
        "If you do, set the environment variable MEDLEYDB_PATH to your "
        "local copy of MedleyDB.",
        UserWarning
    )
    MEDLEYDB_PATH = ""
    AUDIO_AVAILABLE = False
else:
    MEDLEYDB_PATH = environ["MEDLEYDB_PATH"]
    warnings.warn(
        "The value set for MEDLEYDB_PATH: %s does not exist. "
        "As a result, any part of the code that requires the audio won't work. "
        "If you don't need to access the audio, disregard this warning. "
        "If you do, set the environment variable MEDLEYDB_PATH to your local "
        "copy of MedleyDB." % MEDLEYDB_PATH,
        UserWarning
    )
    AUDIO_AVAILABLE = False
