import medleydb as mdb
instruments = ['male singer','female singer']
back_instruments = ['acoustic guitar']

mtrack_generator = mdb.load_all_multitracks()
for mtrack in mtrack_generator:
    track = mdb.MultiTrack(mtrack.track_id)
    for key,stem in track.stems.items():
        if stem.instrument[0] in instruments:
            if stem.ranking == 1 :
                print(stem.audio_path)
        if stem.instrument[0] in back_instruments:
            print(stem.audio_path)
