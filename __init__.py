from . import cfg

class VisualTargeting(object): 
    def __init__(self, user_config):
        cfg.__dict__.update(user_config)
        
        from .app import App
        self.app = App()

    def get(self): 
        self.app.run_single()

        return [f.dump() for f in self.app.active_faces]    