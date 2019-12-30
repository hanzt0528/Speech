import medleydb as mdb
import librosa

instruments = ['male singer', 'female singer']
back_instruments = ['acoustic guitar']

mixtrack=[] 
track_name = ''
mtrack_generator = mdb.load_all_multitracks()
for mtrack in mtrack_generator:
    mixtrack = []
    track = mdb.MultiTrack(mtrack.track_id)
    mixtrack.append(track.track_id)
    for key, stem in track.stems.items():
        if stem.instrument[0] in instruments:
            if stem.ranking == 1:
                mixtrack.append(stem.audio_path)

        if stem.instrument[0] in back_instruments:
            mixtrack.append(stem.audio_path)

    if len(mixtrack) > 2:
        result = None
        sr_out = 0
        track_name = mixtrack.pop(0)
        print(track_name)
        for mt in mixtrack:
            print(mt)
            y, sr = librosa.load(mt, sr=None)
            if result is None:
                result = y
                sr_out = sr
            else:
                result = result + y

        librosa.output.write_wav(track_name + '.wav', result, sr_out)
