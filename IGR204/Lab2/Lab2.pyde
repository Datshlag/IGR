places = []
movingText = 0
bottomText = 0
labelFont = 0
saved = 0

# Classe pour un avoir un texte que l'on peut bouger
class MovingText(object):
    
    s = ""
    posX = 0
    posY = 0
    
    def draw(self):
        text(self.s, self.posX, self.posY)
        
    def setX(self, x):
        self.posX = x
        
    def setY(self, y):
        self.posY = y
        
    def setString(self, string):
        self.s = string

# Classe pour représenter une ville
class Place(object):
    minX, maxX = (0, 0)
    minY, maxY = (0, 0)
    minDensity, maxDensity = (0, 0)
    
    longitude = 0
    latitude = 0
    name = ""
    postalCode = 0
    population = -1
    density = -1
    
    @property
    def x(self):
        return map(self.longitude, self.minX, self.maxX, 0, width)
    
    @property
    def y(self):
        return height - map(self.latitude, self.minY, self.maxY, 0, height)
    @property
    def c(self):
        return floor((255 - map(log(self.density), log(self.minDensity), log(self.maxDensity), 0, 255)) / 10) 
    
    def draw(self):
        try:
            noStroke()
            col = color(255, self.c, 0)
            set(self.x, self.y, col)
            fill(0, 0, 255, 100)
            ellipse(self.x, self.y, self.population/10000, self.population/10000)
        except Exception, e:
            self.longitude = 0
            self.latitude = 0
            print "Error drawing place at ({}, {}): {}".format(self.x, self.y, e)
    
def setup():
    global movingText, bottomText, saved
    
    size(800, 600)
    
    # création du texte mouvant et
    # du texte qui restera en bas à gauche
    movingText = MovingText()
    bottomText = MovingText()
    bottomText.setX(10)
    bottomText.setY(510)
    bottomText.setString("")
    
    # création de la police
    labelFont = loadFont("TlwgTypist-48.vlw")
    textFont(labelFont, 32)
    
    background(255)
    readData()
    # on dessine une première fois
    # puis on ne redessinera jamais cette partie
    for place in places:
            place.draw()
            
    # on récupère la frame après avoir dessiné tous les points
    # afin de ne pas avoir à tout redessiner à chaque fois.
    # On redessinera uniquement les texte, seuls éléments changeants
    saved = get()
    noLoop()
    
def draw():
    # On set le background avec la frame
    # que l'on a sauvegardé
    background(saved) 
    fill(0, 0, 0)
    movingText.draw() 
    bottomText.draw()
    
    
def readData():
    global minX, maxX, minY, maxY
    lines = loadStrings("http://www.infres.enst.fr/~eagan/class/igr204/data/population.tsv")
    print "Loaded", len(lines), "lines"
    
    for line in lines[2:]:
        columns = line.split("\t")
        
        place = Place()
        place.postalCode = int(columns[0])
        place.longitude = float(columns[1])
        place.latitude = float(columns[2])
        place.name = columns[4]
        place.population = int(columns[5])
        place.density = float(columns[6])
        places.append(place)
        
    Place.minX = min(places, key=lambda place: place.longitude).longitude
    Place.maxX = max(places, key=lambda place: place.longitude).longitude
    Place.minY = min(places, key=lambda place: place.latitude).latitude
    Place.maxY = max(places, key=lambda place: place.latitude).latitude
    Place.minDensity = min(places, key=lambda place: place.density).density
    Place.maxDensity = max(places, key=lambda place: place.density).density
    
def mouseMoved():
    
    # On récupère la ville la plus proche
    # de la souris, et ses charactéristiques
    movingText.setX(mouseX)
    movingText.setY(mouseY)
    distance, name, density, population = getClosestCity(mouseX, mouseY)
    
    # Si on est suffisamment près
    # (i.e pas dans l'océan), on affiche 
    # le nom et les charactéristiques de la ville
    if(distance < 5):
        movingText.setString(name)
        bottomText.setString(name + "\nPopulation : " + str(population) + "\nDensity : " + str(density))
    else:
        movingText.setString("")
        bottomText.setString("")
    # on redessine
    redraw()

def getClosestCity(posX, posY):
    
    # on récupère la ville la plus proche de (posX, posY)
    mini = 100
    argmin = 0
    
    for i in range(len(places)):
        place = places[i]
        distance = sqrt((place.x - posX)**2 + (place.y - posY)**2)
        
        if(distance < mini):
            mini = distance
            argmin = i
            
    return mini, places[argmin].name, places[argmin].density, places[argmin].population       