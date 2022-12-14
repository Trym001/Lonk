
class regulator:
    def __init__(self):
        self.p = 2
        self.i = 0.1
        self.diff = 0
        self.u = -self.p * self.diff

    def update_u(self):
        self.u = -self.p * self.diff

    def p_regulator(self, left_sens: int, right_sens: int, yaw: int):
        self.diff_converting(left_sens, right_sens)
        self.update_u()
        new_heading = (yaw + self.u) % 360
        return new_heading

    def pi_regulator(self, left_sens, right_sens, yaw):
        self.diff_converting(left_sens, right_sens)
        self.update_u()
        self.u += self.i * self.diff
        new_heading = int((yaw + self.u) % 360)
        return new_heading

    def diff_converting(self, left_sens: int, right_sens: int):
        self.diff = left_sens/10 - right_sens/10
