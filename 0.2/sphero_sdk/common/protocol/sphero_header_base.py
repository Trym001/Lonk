#!/usr/bin/env python3


class SpheroHeaderBase:
    def __init__(self):
        pass

    @staticmethod
    def from_buffer(buf):
        raise NotImplementedError

    def serialise(self):
        raise NotImplementedError
