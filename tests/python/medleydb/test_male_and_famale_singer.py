import medleydb as mdb

instruments = ['female singer','male singer']

mtrack_generator=mdb.load_all_multitracks()
for mtrack in mtrack_generator:
    track=mdb.MultiTrack(mtrack.track_id)
    for key,stem in track.stems.items():
        if stem.instrument[0] in instruments:
            print(stem.ranking,mtrack.track_id)
            
