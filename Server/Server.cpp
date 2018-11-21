#include "pch.h"
#pragma warning( disable : 4996)
#pragma comment (lib, "ws2_32.lib")
using namespace std;

fd_set global_set;
SOCKET listening_socket;
WSADATA wsData;
WORD ver = MAKEWORD(2, 2);
sockaddr_in hint;

string hostname;
string default_prefix = "root@localhost";
string prefix = default_prefix;
string randomWords[4700] = { "asol","aahed", "aargh", "abaci", "abaft", "abamp", "abash", "abaya", "abbes", "abbot", "abele", "abhor", "abled", "ables", "abode", "aboil", "aboon", "about", "abris", "abuts", "abyes", "abyss", "acerb", "ached", "achoo", "acidy", "acini", "acmes", "acned", "acock", "acorn", "acres", "acted", "actor", "acyls", "adapt", "added", "addle", "adept", "adios", "adman", "admit", "adobe", "adopt", "adorn", "adoze", "adunc", "adyta", "adzes", "aedes", "aeons", "afars", "afire", "afore", "afrit", "again", "agape", "agate", "agaze", "agent", "agger", "aggro", "agile", "agios", "agist", "aglee", "agley", "agmas", "agons", "agora", "agria", "ahead", "ahold", "aided", "aides", "aimed", "aioli", "airer", "airth", "aisle", "aiver", "ajuga", "akela", "alack", "aland", "alang", "alant", "alary", "albas", "alcid", "aldol", "alefs", "alert", "algae", "algas", "algin", "algum", "alibi", "alifs", "alike", "alist", "aliya", "alkyd", "allay", "alley", "allot", "alloy", "almah", "almeh", "almud", "aloes", "aloha", "alone", "aloof", "alpha", "alter", "altos", "alums", "amahs", "amass", "amber", "amble", "ambry", "ameer", "amens", "amias", "amici", "amido", "amies", "amigo", "amino", "amirs", "amity", "amnia", "amnio", "amole", "amort", "amped", "amply", "amuck", "amyls", "ancon", "anear", "anent", "angel", "angle", "angry", "anile", "anima", "animi", "anise", "ankle", "anlas", "annas", "annoy", "anoas", "anole", "ansae", "antas", "antes", "antis", "antre", "anvil", "aorta", "apart", "apeek", "apery", "aphis", "aping", "apnea", "aport", "appel", "apply", "apron", "apsis", "aptly", "aquas", "arame", "arced", "ardeb", "areae", "areas", "areic", "arene", "arete", "argil", "argol", "argot", "argus", "arias", "arils", "arles", "armer", "armor", "aroma", "arpen", "array", "arrow", "arsis", "artal", "artsy", "arval", "aryls", "ascot", "asdic", "ashen", "aside", "asker", "askoi", "aspen", "aspic", "assai", "asses", "aster", "asyla", "ataxy", "atlas", "atmas", "atoms", "atone", "atopy", "atrip", "attic", "audio", "auger", "augur", "aunts", "aurae", "aurar", "aurei", "auric", "aurum", "auxin", "avant", "avens", "avert", "avian", "aviso", "avows", "awake", "aware", "awful", "awned", "awols", "axial", "axils", "axiom", "axite", "axles", "axmen", "axons", "ayins", "azide", "azine", "azoic", "azons", "azoth", "azure", "baals", "babel", "babka", "babul", "bacca", "bacon", "badge", "baffs", "bagel", "bahts", "bairn", "baits", "baize", "baker", "balas", "baldy", "baler", "balks", "balls", "balms", "balsa", "banco", "bands", "baned", "bangs", "banks", "banty", "barbs", "barde", "bared", "bares", "barge", "barks", "barms", "barns", "baron", "barye", "based", "bases", "basil", "basis", "bassi", "bassy", "basts", "bated", "bathe", "batik", "batts", "batty", "baulk", "bawdy", "bawty", "bayou", "bazoo", "beads", "beaks", "beams", "beano", "beard", "beast", "beaus", "beaux", "becap", "bedel", "bedim", "beedi", "beefy", "beers", "beets", "befog", "begat", "begin", "begum", "beige", "being", "belch", "belie", "bells", "belon", "belts", "bemix", "bends", "benes", "benni", "bento", "beret", "berks", "berms", "berth", "beses", "besom", "bests", "betel", "beton", "bevel", "bewig", "bezil", "bhoot", "biali", "bibbs", "bicep", "biddy", "bider", "bidet", "bield", "biffs", "bifid", "bight", "bigos", "bijou", "biker", "bikie", "bilby", "bilge", "bilks", "billy", "bimas", "binal", "binds", "bines", "bingo", "bints", "biome", "biota", "bipod", "birds", "birle", "biros", "birse", "bises", "bison", "biter", "bitsy", "bitty", "blabs", "blade", "blahs", "blame", "bland", "blare", "blast", "blats", "blaws", "bleak", "bleat", "bleed", "blend", "bless", "blets", "blimy", "blini", "blips", "blite", "bloat", "block", "blogs", "blond", "bloom", "blots", "blows", "blubs", "bluer", "bluet", "bluff", "blunt", "blurs", "blush", "board", "boart", "boats", "bocce", "boche", "boded", "boffo", "bogan", "boggy", "bogle", "bohea", "boils", "boink", "bolar", "bolds", "bolls", "bolts", "bombe", "bonds", "boner", "boney", "bongs", "bonne", "bonus", "boobs", "boody", "boogy", "booms", "boons", "boost", "boots", "booze", "boral", "borax", "borer", "boric", "borne", "borts", "bortz", "bosky", "boson", "bosun", "botch", "bothy", "bough", "bound", "bourn", "bousy", "bovid", "bowel", "bowls", "boxed", "boxes", "boyla", "bozos", "brach", "brads", "brags", "brail", "brake", "brand", "brans", "brash", "brats", "brave", "bravo", "brawn", "braxy", "braza", "bread", "bream", "breed", "brens", "breve", "briar", "brick", "brief", "bries", "brill", "brine", "brink", "briny", "brisk", "brith", "britt", "brock", "broke", "bromo", "brood", "broom", "brose", "broth", "brows", "bruin", "brume", "brunt", "brusk", "bruts", "bubba", "bubus", "bucks", "budge", "buffo", "buffy", "bugle", "buhrs", "built", "bulge", "bulks", "bulla", "bully", "bumph", "bumpy", "bunch", "bunds", "bungs", "bunks", "bunny", "bunya", "buppy", "buras", "burds", "burgh", "burin", "burke", "burly", "burnt", "burqa", "burrs", "bursa", "burst", "bused", "bushy", "busts", "butch", "butes", "butte", "butty", "butyl", "buyer", "bylaw", "byrls", "bytes", "cabal", "caber", "cable", "cacao", "cache", "caddy", "cadet", "cadgy", "cadre", "cafes", "caged", "cages", "cahow", "cains", "cairn", "caked", "cakey", "calif", "calks", "calls", "calos", "calyx", "camel", "cames", "campi", "camps", "canal", "caned", "canes", "canna", "canoe", "canso", "canto", "canty", "caper", "caphs", "capon", "caput", "carbo", "cards", "carer", "caret", "cargo", "carle", "carns", "carob", "carom", "carps", "carry", "carte", "carve", "cased", "casks", "caste", "casus", "cater", "catty", "caulk", "cause", "caver", "cavie", "cawed", "cebid", "cecum", "ceded", "cedes", "ceiba", "ceils", "cella", "cello", "celom", "cense", "cents", "ceorl", "cerci", "ceres", "ceric", "cesta", "cetes", "chafe", "chain", "chais", "champ", "chang", "chaos", "chaps", "chard", "chark", "charr", "chart", "chase", "chats", "chays", "cheat", "cheek", "cheer", "chela", "chert", "chest", "chevy", "chewy", "chias", "chick", "chics", "chief", "child", "chili", "chimb", "chimp", "chine", "chino", "chips", "chirm", "chirp", "chiru", "chive", "chock", "choke", "chola", "chomp", "chops", "chore", "chott", "chubs", "chufa", "chugs", "chums", "churl", "churr", "chyle", "cibol", "cigar", "cimex", "cines", "circa", "cirri", "cissy", "cited", "cites", "civic", "civil", "clach", "clade", "clags", "clamp", "clang", "clans", "clapt", "clary", "clasp", "clast", "clavi", "clays", "clear", "cleek", "cleft", "clept", "clews", "cliff", "climb", "cline", "clink", "clipt", "clock", "clogs", "clomp", "clonk", "cloot", "close", "clots", "clour", "clove", "cloys", "clubs", "clued", "clump", "clunk", "coach", "coala", "coaly", "coast", "coats", "cobby", "coble", "cocas", "cocks", "cocoa", "codas", "coded", "coder", "codex", "coeds", "cogon", "cohos", "coign", "coins", "coked", "colas", "colds", "coles", "colin", "colog", "color", "colza", "comal", "combe", "combs", "comes", "comfy", "comix", "commy", "comps", "comte", "condo", "cones", "conga", "congo", "conin", "conky", "conte", "conus", "cooed", "cooer", "coofs", "cooky", "cooly", "coons", "coopt", "copal", "coped", "coper", "copra", "coral", "cords", "corer", "corgi", "corks", "corms", "cornu", "corps", "cosec", "coset", "cosie", "costs", "coted", "cotta", "coude", "could", "coupe", "court", "coved", "cover", "covet", "covin", "cower", "cowry", "coxal", "coxes", "coyer", "coypu", "cozes", "cozie", "crabs", "craft", "crake", "crams", "crank", "craps", "crass", "crave", "craws", "crazy", "cream", "creds", "creek", "creep", "crepe", "crepy", "crest", "cribs", "cried", "cries", "crimp", "crisp", "croak", "crock", "croft", "crony", "croon", "crore", "croup", "crown", "croze", "crude", "cruel", "crumb", "cruor", "cruse", "crust", "crypt", "cubeb", "cuber", "cubic", "cuddy", "cuifs", "cuish", "culch", "culex", "cully", "culpa", "cults", "cunts", "cupid", "cuppy", "curch", "curdy", "curer", "curet", "curia", "curio", "curly", "currs", "curse", "curve", "cusec", "cusks", "cusso", "cuter", "cutey", "cutin", "cutty", "cuvee", "cyans", "cycad", "cycle", "cyder", "cymae", "cymas", "cymol", "cysts", "czars", "dacha", "daddy", "daffs", "dagga", "dahls", "dairy", "dales", "daman", "dames", "damps", "dandy", "danio", "dared", "dares", "darks", "darns", "dashi", "dated", "dates", "datto", "daube", "dauby", "dauts", "davit", "dawen", "dawns", "dazed", "deads", "deals", "deans", "deary", "death", "debag", "debit", "debug", "debye", "decal", "decks", "decos", "decry", "deeds", "deems", "deers", "defat", "defis", "degas", "deice", "deign", "deism", "deity", "dekes", "delay", "deles", "delft", "dells", "delta", "delve", "demic", "demob", "demos", "denar", "denim", "dents", "depot", "derat", "derby", "derms", "desex", "deter", "deuce", "devel", "devon", "dewar", "dewed", "dexie", "dhals", "dhole", "dhows", "dials", "diazo", "dicer", "dicey", "dicky", "dicta", "didie", "didst", "diets", "dight", "diked", "dikes", "dildo", "dilly", "dimes", "dinar", "diner", "dinge", "dings", "dinks", "dinos", "diode", "dippy", "diram", "dirge", "dirls", "dirty", "disco", "dishy", "disme", "ditch", "ditsy", "ditty", "divan", "dived", "dives", "divvy", "dixit", "dizzy", "djins", "dobby", "dobla", "dobro", "dodge", "dodos", "doest", "doffs", "dogey", "doggy", "dogma", "doing", "dojos", "dolci", "doles", "dolly", "dolor", "domal", "domes", "donas", "donga", "donna", "donor", "donut", "dooly", "doomy", "doozy", "doped", "dopes", "dorks", "dorms", "dorps", "dorsa", "dosed", "doses", "doted", "dotes", "doubt", "dough", "douma", "doura", "doven", "dowdy", "dowel", "dowie", "downy", "dowse", "doyen", "dozed", "dozer", "drabs", "draft", "drail", "drake", "drams", "drape", "drave", "drawn", "drays", "dream", "dreck", "drees", "dreks", "drest", "dried", "dries", "drill", "drink", "dript", "droid", "droll", "drool", "drops", "dross", "drove", "drubs", "druid", "drunk", "druse", "dryer", "duads", "ducal", "duces", "ducks", "ducts", "duded", "duels", "duffs", "duits", "dukes", "dulls", "dulse", "dumbo", "dumka", "dummy", "dumpy", "dunce", "dunes", "dungy", "dunts", "duomo", "duper", "duple", "duras", "dures", "duroc", "durra", "durst", "dusks", "dusts", "dutch", "dwarf", "dwell", "dwine", "dyers", "dyked", "dykey", "dynes", "eagle", "eared", "early", "earth", "easel", "easts", "eater", "eaves", "ebbet", "ebony", "eched", "echos", "ecrus", "edged", "edges", "edify", "edits", "educt", "egads", "egest", "egged", "egret", "eidos", "eikon", "eking", "eland", "elate", "elder", "elegy", "elfin", "elint", "eloin", "elude", "elver", "email", "embay", "ember", "emcee", "emend", "emeus", "emits", "emmet", "emote", "emyde", "enact", "ended", "endow", "enema", "enjoy", "enoki", "enorm", "enrol", "ensue", "entia", "enure", "envoy", "eosin", "epees", "ephas", "ephor", "epoch", "epoxy", "equid", "erase", "ergot", "ernes", "erose", "error", "eruct", "erupt", "escar", "eskar", "esnes", "esses", "estop", "ether", "ethos", "etnas", "etuis", "etyma", "evade", "event", "every", "evils", "evoke", "exact", "exams", "execs", "exile", "exing", "exits", "expat", "expos", "extra", "exult", "eyass", "eying", "eyres", "eyrir", "faced", "faces", "facia", "faddy", "fader", "fadge", "faena", "faggy", "fagot", "faint", "fairy", "faked", "fakes", "fakir", "false", "fames", "fanes", "fangs", "fanon", "fanum", "farad", "farci", "fards", "farer", "farle", "farms", "farts", "fatal", "fates", "fatso", "fatwa", "fauld", "fauna", "fauve", "faves", "favus", "fawny", "faxes", "fazed", "fears", "feast", "feaze", "feces", "fedex", "feeds", "feeze", "feint", "felid", "fells", "felon", "femes", "femur", "fends", "feods", "feral", "feria", "fermi", "ferny", "fesse", "fetal", "fetch", "fetes", "fetor", "feuar", "feued", "fewer", "feyly", "fezzy", "fiats", "fibre", "fiche", "ficin", "fidge", "fiefs", "fiend", "fifed", "fifes", "fifty", "filar", "filed", "files", "fille", "fills", "filmi", "filmy", "filth", "final", "finch", "fined", "fines", "finks", "finos", "fique", "firer", "firms", "firry", "firth", "fishy", "fitch", "fiver", "fixed", "fixes", "fizzy", "fjord", "flack", "flail", "flake", "flame", "flamy", "flans", "flare", "flask", "flaws", "flaxy", "fleam", "fleck", "flees", "flesh", "fleys", "flics", "flier", "fling", "flips", "flirt", "flits", "flock", "floes", "flong", "floor", "flora", "flota", "flout", "flows", "flued", "fluff", "fluke", "flume", "flung", "fluor", "flute", "fluyt", "flyer", "foals", "foamy", "focus", "fogey", "fogie", "foils", "foist", "foley", "folic", "folks", "folly", "fondu", "foods", "foots", "foram", "forbs", "force", "fords", "forge", "forks", "forme", "forte", "forts", "forum", "fosse", "found", "fours", "fowls", "foxes", "frags", "frame", "frank", "frass", "fraud", "freak", "freer", "fremd", "frere", "frets", "fried", "fries", "frill", "frisk", "frits", "fritz", "frock", "frogs", "frons", "frore", "frost", "frown", "froze", "fruit", "fryer", "fubsy", "fucus", "fudge", "fugal", "fugio", "fugue", "fujis", "fully", "fumer", "fumet", "funds", "fungo", "funky", "furan", "furor", "furze", "fused", "fusel", "fusil", "fusty", "fuzed", "fuzes", "fuzzy", "fykes", "gabby", "gaddi", "gadis", "gadjo", "gaffs", "gager", "gaily", "gaits", "galas", "galea", "galls", "galop", "gamay", "gambe", "gamed", "games", "gamic", "gamma", "gamps", "ganef", "gangs", "ganof", "gaped", "gapes", "garbs", "garni", "gases", "gassy", "gated", "gates", "gauds", "gauge", "gaums", "gaurs", "gauze", "gavel", "gawks", "gawps", "gayal", "gayly", "gazed", "gazes", "gears", "gecks", "geeky", "geest", "gelee", "gelts", "gemmy", "genes", "genic", "genii", "genoa", "genre", "gents", "genus", "geoid", "germs", "gesso", "gests", "getup", "ghast", "ghaut", "ghees", "ghoul", "giant", "giber", "giddy", "gigas", "gigot", "gilds", "gilly", "gimel", "gimps", "ginks", "ginzo", "gipsy", "girls", "girns", "giros", "girth", "gismo", "gites", "giver", "gizmo", "glade", "glady", "glams", "glans", "glary", "glaze", "gleam", "gleba", "glede", "gleed", "glees", "glens", "glial", "glide", "glime", "glint", "gloam", "globe", "glogg", "gloom", "glory", "glost", "glove", "gloze", "gluer", "gluey", "glume", "gluon", "gluts", "gnarl", "gnars", "gnats", "gnaws", "goads", "goats", "gobos", "godly", "gofer", "going", "golem", "golly", "gomer", "gonef", "gongs", "gonif", "gonzo", "goody", "goofs", "gooks", "goons", "goops", "goose", "gopik", "gored", "gorge", "gorps", "gorsy", "gouge", "gouts", "gowan", "gowks", "goxes", "graal", "grace", "grads", "grail", "grama", "grams", "grand", "grant", "graph", "grasp", "grate", "gravy", "graze", "grebe", "greek", "grees", "grego", "gride", "grief", "grift", "grill", "grimy", "grins", "gripe", "gript", "grist", "grits", "groat", "grogs", "groks", "grope", "grosz", "group", "grove", "grown", "grubs", "grues", "grume", "grunt", "guano", "guard", "guava", "gudes", "guest", "guide", "guild", "guilt", "guise", "gular", "gules", "gulfy", "gully", "gulpy", "gumma", "gunks", "gunny", "gurge", "gursh", "gushy", "gusto", "gusty", "gutta", "guyed", "gwine", "gybes", "gypsy", "gyred", "gyron", "gyrus", "gyves", "haars", "habus", "hacks", "haded", "hadji", "haems", "hafiz", "hahas", "haiks", "hails", "hairs", "hajes", "hajji", "hakim", "halal", "haler", "halid", "halls", "halms", "halos", "halva", "hamal", "hammy", "hance", "handy", "hanks", "hansa", "hants", "hapax", "happy", "hardy", "harem", "harks", "harms", "harpy", "harsh", "hasps", "hasty", "hated", "hates", "haulm", "haunt", "haven", "haves", "hawed", "hawse", "hayer", "hazan", "hazel", "hazes", "heady", "heaps", "heard", "heart", "heats", "heavy", "hecks", "hedge", "heeds", "heeze", "hefty", "heils", "heist", "helix", "hells", "helos", "helps", "hemal", "hemic", "hemps", "hence", "henna", "hents", "herby", "heres", "herma", "herns", "heros", "hertz", "heths", "heugh", "hewer", "hexed", "hexes", "hicks", "hider", "highs", "hijab", "hiked", "hikes", "hillo", "hilly", "hilum", "hinds", "hinky", "hints", "hippo", "hired", "hirer", "hissy", "hitch", "hives", "hoard", "hoary", "hobos", "hocus", "hoers", "hoggs", "hoise", "hoked", "hokey", "hokum", "holed", "holey", "holla", "holly", "holts", "homer", "homey", "homos", "honda", "honer", "honey", "hongs", "honky", "hooch", "hoody", "hoofs", "hooks", "hooly", "hoots", "hoped", "hopes", "horah", "horas", "horns", "horse", "horsy", "hosel", "hoser", "hosey", "hosts", "hotel", "hound", "hours", "hovel", "howdy", "howff", "howks", "hoyas", "hubby", "huffs", "huger", "hulks", "hullo", "human", "humid", "humph", "humpy", "hunch", "hunky", "hurds", "hurly", "hurst", "husks", "hussy", "huzza", "hydro", "hying", "hymen", "hyoid", "hyper", "hypha", "hyrax", "iambi", "ichor", "icily", "icker", "ictic", "ideal", "idiom", "idled", "idles", "idyll", "igged", "iglus", "ikats", "ileac", "ileum", "iliac", "ilial", "iller", "imago", "imaum", "imbue", "imido", "imine", "immix", "impel", "imply", "inapt", "inbye", "incur", "index", "indol", "indri", "inept", "infer", "infos", "ingle", "inion", "inker", "inlay", "inned", "input", "inset", "intis", "inure", "invar", "iodid", "ionic", "irade", "irids", "irked", "irone", "irony", "isled", "islet", "issue", "itchy", "ither", "ivies", "ixias", "ixtle", "jabot", "jacks", "jaded", "jager", "jaggy", "jails", "jalap", "jambe", "jammy", "janty", "japed", "japes", "jatos", "jaunt", "javas", "jawed", "jeans", "jeeps", "jefes", "jehus", "jells", "jemmy", "jerid", "jerky", "jesse", "jetes", "jetty", "jewel", "jibed", "jibes", "jiffy", "jihad", "jilts", "jimpy", "jinks", "jinns", "jived", "jives", "jnana", "jocks", "johns", "joint", "joked", "jokes", "joles", "jolts", "jomon", "joram", "jotas", "joual", "joule", "jowar", "jowls", "joyed", "jubes", "judas", "judos", "jugum", "juicy", "juked", "jukus", "jumbo", "jumpy", "junks", "junta", "jupes", "jural", "jurel", "justs", "jutty", "kabar", "kadis", "kagus", "kaifs", "kains", "kakis", "kales", "kalpa", "kamik", "kanes", "kanzu", "kapas", "kapok", "kaput", "karma", "karoo", "karts", "katas", "kaury", "kayak", "kazoo", "kebab", "kebob", "kedge", "keeks", "keens", "keets", "kefir", "kelep", "kelly", "kelpy", "kemps", "kenaf", "kendo", "kente", "kerbs", "kerne", "kerry", "ketol", "kevil", "keyed", "khafs", "khans", "khats", "kheth", "khoum", "kibbe", "kibei", "kibla", "kicky", "kiddy", "kiers", "kilim", "kilns", "kilts", "kinas", "kines", "kinin", "kinky", "kiosk", "kirns", "kists", "kiter", "kithe", "kitty", "kiwis", "kliks", "kloof", "klutz", "knaps", "knaur", "knawe", "kneed", "knees", "knelt", "knish", "knobs", "knoll", "knosp", "knout", "knows", "knurs", "koans", "koels", "koine", "kolas", "kombu", "kooks", "kopek", "kopje", "korai", "korat", "korun", "kotow", "kraft", "kraut", "krewe", "krona", "kroon", "kudos", "kudzu", "kugel", "kulak", "kurta", "kusso", "kvell", "kyaks", "kyats", "kyrie", "kythe", "label", "labor", "laced", "laces", "lacks", "laden", "lades", "laevo", "lager", "laich", "laigh", "lairs", "laity", "laker", "lakhs", "lamas", "lamby", "lamer", "lamia", "lanai", "lands", "lanky", "lapin", "lapse", "lards", "laree", "large", "laris", "larky", "larva", "laser", "lassi", "lasts", "lated", "later", "lathe", "laths", "latke", "lauan", "laugh", "lavas", "laver", "lawed", "lawny", "laxes", "layed", "layin", "lazar", "lazes", "leads", "leafs", "leaks", "leans", "leaps", "learn", "leary", "leash", "leave", "leben", "ledgy", "leeks", "leery", "lefts", "legal", "leges", "legit", "lehua", "lemma", "lemur", "lenes", "lenos", "lento", "leper", "lesbo", "letch", "letup", "levee", "lever", "levis", "lexes", "lezzy", "liane", "liard", "libel", "libra", "lichi", "licit", "lidar", "liege", "liers", "lieve", "lifts", "liger", "liked", "liker", "lilac", "lilts", "limas", "limbi", "limbs", "limed", "limes", "limit", "limos", "limps", "lindy", "linen", "lines", "linga", "lings", "linin", "linky", "linos", "linty", "lions", "lipin", "liras", "lisle", "lists", "litas", "lithe", "litre", "liven", "lives", "livre", "llano", "loads", "loams", "loans", "lobar", "lobed", "lobos", "lochs", "locos", "locus", "lodes", "loess", "lofty", "loges", "logia", "login", "logon", "loids", "lolls", "loner", "longs", "looed", "loofa", "looie", "looms", "loony", "loopy", "loots", "loper", "loppy", "loran", "lores", "lorry", "loser", "lossy", "lotas", "lotos", "lotto", "lough", "louis", "loupe", "lours", "louse", "louts", "loved", "loves", "lower", "lowly", "loxed", "loyal", "lubed", "luces", "lucks", "lucre", "ludic", "luffs", "luger", "lulls", "lumas", "lumps", "lunar", "lunch", "lunet", "lungi", "lunks", "lupin", "lurch", "lurer", "lurex", "lurks", "lusty", "lutea", "lutes", "lweis", "lyart", "lycea", "lycra", "lymph", "lyres", "lysed", "lysin", "lyssa", "lytta", "mabes", "maced", "maces", "macho", "macks", "macon", "madam", "madre", "mafic", "magic", "magot", "mahoe", "maile", "mails", "mains", "maist", "major", "maker", "makos", "males", "malls", "malmy", "malty", "mamba", "mamey", "mamma", "manas", "maned", "manga", "mango", "mania", "manly", "manor", "manse", "manus", "maqui", "march", "mares", "maria", "marks", "marly", "marse", "marts", "masas", "mashy", "mason", "masse", "masts", "mated", "mates", "maths", "matte", "matza", "mauds", "maund", "mauve", "mavie", "mavis", "maxed", "maxim", "mayan", "maybe", "mayor", "mayst", "mazer", "mbira", "meals", "means", "meany", "meaty", "medal", "medic", "meeds", "meiny", "melee", "mells", "melts", "memes", "menad", "mensa", "mensh", "menus", "meows", "mercs", "merde", "meres", "merit", "merle", "merry", "meshy", "mesne", "messy", "meted", "metes", "metis", "metre", "mewed", "mezes", "miaou", "miasm", "micas", "micks", "micro", "midge", "midst", "miffs", "miggs", "miked", "mikra", "milds", "miles", "milks", "mille", "milos", "milts", "mimed", "mimer", "mimic", "minas", "mincy", "mined", "mines", "minim", "minke", "minny", "mints", "minus", "mires", "mirin", "mirky", "mirza", "miser", "misos", "mists", "miter", "mitis", "mitts", "mixer", "mixup", "moans", "mocha", "modal", "modem", "modus", "mogul", "mohur", "moira", "moist", "mokes", "molar", "molds", "moles", "molly", "molts", "momma", "momus", "monas", "mondo", "mongo", "monks", "monte", "mooch", "moody", "moola", "moons", "moors", "moose", "moped", "mopes", "morae", "moras", "morel", "morns", "morph", "morse", "mosey", "mosso", "moste", "motel", "motet", "moths", "motif", "motte", "motts", "moues", "moult", "mount", "mouse", "mouth", "mover", "movie", "mower", "moxie", "mucho", "mucin", "mucky", "mucro", "muddy", "muffs", "muggs", "muhly", "mulch", "muled", "muley", "mulls", "mummy", "mumus", "mungo", "muons", "muras", "mures", "murid", "murky", "murre", "murry", "mused", "muses", "music", "musky", "musth", "musty", "muted", "mutes", "mutts", "mylar", "mynas", "myoma", "myopy", "mysid", "mythy", "nabes", "nabob", "nacre", "nadir", "naffs", "naiad", "nails", "nairu", "naked", "nalas", "named", "names", "nance", "nanny", "napes", "nappe", "narco", "nards", "naric", "narks", "nasal", "natal", "nates", "naval", "navel", "navvy", "nazis", "nears", "neats", "neddy", "needy", "neeps", "neifs", "neist", "nemas", "neons", "nerdy", "nerts", "nerve", "nests", "netts", "neuks", "neums", "neves", "newel", "newie", "newsy", "nexus", "nicad", "niche", "nicol", "nided", "nidus", "nieve", "nighs", "nihil", "nimbi", "ninja", "ninon", "nipas", "nisei", "niter", "nitid", "nitre", "nitty", "nixed", "nixie", "nobby", "nobly", "nodal", "nodes", "noels", "nohow", "noily", "noise", "nolos", "nomas", "nomes", "nomos", "nonce", "nonet", "nooks", "noons", "nopal", "noris", "north", "noses", "notal", "noted", "notes", "nouns", "novas", "noway", "nubby", "nucha", "nudes", "nudie", "nuked", "nulls", "numen", "nurls", "nutsy", "nyala", "nymph", "oakum", "oases", "oasts", "oater", "oaves", "obeli", "obeys", "obits", "oboes", "oboli", "occur", "ocher", "ochry", "ocrea", "octal", "octet", "oculi", "odder", "odeon", "odist", "odors", "odyle", "ofays", "offed", "often", "ogams", "ogham", "ogled", "ogles", "ohias", "ohmic", "oiled", "oinks", "okays", "okras", "older", "oleic", "oleos", "olios", "ollas", "omasa", "ombre", "omens", "omits", "onery", "onium", "onset", "oohed", "oorie", "oozed", "opahs", "opens", "opine", "opium", "opted", "orach", "orang", "orbed", "orcas", "order", "oread", "orgic", "oriel", "orlon", "ormer", "orpin", "ortho", "osier", "osmol", "ostia", "ottar", "ottos", "ounce", "ouphs", "ousel", "outby", "outed", "outgo", "ouzel", "ovals", "ovate", "overs", "ovine", "ovoli", "ovule", "owlet", "owner", "oxbow", "oxide", "oxime", "oxlip", "oyers", "pacas", "pacer", "pacey", "packs", "paddy", "padle", "padri", "paeon", "paged", "pages", "paiks", "pains", "pairs", "paise", "paled", "pales", "palls", "palms", "palpi", "palsy", "panda", "paned", "panes", "pangs", "panne", "panto", "panty", "papas", "paper", "pappy", "paras", "pardi", "pardy", "pareo", "pares", "parge", "paris", "parks", "parol", "parry", "parts", "parve", "paseo", "pasha", "pasta", "pasts", "patch", "paten", "pates", "patin", "patly", "patty", "pavan", "paver", "pavid", "pavis", "pawer", "pawls", "paxes", "payee", "payor", "peach", "peags", "peaky", "peans", "pears", "pease", "peaty", "pecan", "pecks", "pedal", "pedro", "peels", "peeps", "peery", "peins", "pekan", "pekin", "peles", "pelon", "penal", "pends", "pengo", "penna", "penni", "peons", "pepla", "peppy", "perdu", "perea", "peril", "perks", "perms", "perry", "pervs", "pesos", "pests", "petal", "petit", "petto", "pewee", "phage", "phial", "phone", "phons", "photo", "phpht", "phyla", "piano", "pibal", "picas", "picky", "picul", "piers", "piety", "pigmy", "pikas", "piker", "pikis", "pilar", "pilaw", "piled", "piles", "pills", "pilus", "pimps", "pinch", "pines", "pingo", "pinko", "pinky", "pinny", "pinot", "pinto", "pinup", "pious", "piped", "pipes", "pipit", "pirns", "pisco", "piste", "pitch", "pithy", "pitta", "pixel", "pixie", "place", "plage", "plain", "plane", "plans", "plash", "plate", "platy", "plays", "plead", "pleat", "plebs", "pleon", "plica", "plier", "plink", "plonk", "plots", "plows", "pluck", "plumb", "plump", "plumy", "plush", "poach", "pocks", "podgy", "poems", "poets", "poilu", "point", "poked", "pokes", "polar", "poler", "polio", "polka", "polos", "polys", "pommy", "pomps", "ponds", "pongs", "poods", "poofs", "poohs", "poons", "poori", "popes", "poppy", "porch", "pores", "porks", "porno", "porny", "posed", "poses", "posse", "potsy", "potty", "pouff", "poult", "pours", "pouty", "poxed", "poyou", "prahu", "prang", "praos", "prate", "praus", "prays", "preen", "preop", "presa", "press", "prexy", "price", "pricy", "pried", "pries", "prill", "prime", "primo", "prims", "print", "prior", "prism", "privy", "proas", "prods", "profs", "prole", "proms", "prong", "props", "proso", "prost", "proud", "prowl", "proxy", "prune", "pryer", "pseud", "psoae", "psoas", "pubes", "pubis", "pucka", "pudgy", "puffs", "puggy", "pujas", "pukes", "puled", "pules", "pulis", "pulps", "pulse", "pumps", "punch", "punji", "punks", "punny", "punts", "pupae", "pupas", "puppy", "purda", "purer", "purin", "purls", "purse", "purty", "pushy", "puton", "putto", "putty", "pyins", "pyoid", "pyres", "pyric", "pyxes", "pyxis", "qaids", "qophs", "quads", "quags", "quais", "quaky", "qualm", "quare", "quart", "quasi", "quate", "qubit", "queen", "quell", "query", "queue", "quick", "quiet", "quill", "quins", "quips", "quire", "quirt", "quits", "quoin", "quoll", "quote", "qursh", "rabbi", "rabid", "racer", "racks", "radar", "radio", "radon", "rafts", "raged", "rages", "raggy", "raias", "rails", "rainy", "raita", "rajas", "raked", "raker", "rakis", "rales", "ralph", "ramee", "ramet", "rammy", "ramus", "ranch", "randy", "range", "ranid", "ranks", "raped", "rapes", "rapid", "rarer", "rased", "rases", "raspy", "ratan", "rated", "rater", "rathe", "ratos", "raved", "raven", "raves", "rawer", "rawly", "raxes", "rayas", "rayon", "razee", "razes", "reach", "readd", "ready", "reals", "reaps", "rears", "reave", "rebbe", "rebel", "rebop", "rebut", "recap", "recit", "recon", "recti", "recur", "redan", "reded", "redia", "redip", "redon", "redox", "redub", "redye", "reedy", "reefy", "reeky", "reest", "refed", "refer", "refix", "refry", "reges", "regna", "rehem", "reify", "reink", "reive", "rekey", "relay", "relic", "reman", "remet", "remit", "renal", "renew", "renin", "rents", "repay", "repel", "reply", "repot", "repro", "rerig", "resat", "resay", "reset", "resid", "resit", "resow", "retag", "retch", "retia", "retro", "reuse", "revet", "rewan", "rewed", "rewin", "rexes", "rheme", "rhino", "rhumb", "rhyta", "riant", "ribby", "riced", "rices", "ricks", "rides", "ridgy", "rifer", "rifle", "right", "rigor", "riles", "rille", "rimed", "rimes", "rindy", "rinks", "rioja", "riped", "riper", "risen", "rises", "risks", "risus", "ritzy", "rived", "river", "rivet", "roach", "roams", "roars", "robed", "robin", "robot", "rocky", "rodes", "rogue", "roily", "rolfs", "roman", "romps", "roods", "rooks", "rooms", "roose", "roots", "roped", "ropes", "roque", "roses", "roshi", "rotas", "rotes", "rotls", "rotos", "rouen", "rouge", "round", "roupy", "roust", "routh", "roved", "rover", "rowan", "rowed", "rowen", "rowth", "ruana", "rubel", "ruble", "ruche", "rudds", "ruder", "ruffe", "rugae", "rugby", "ruins", "ruler", "rumba", "rummy", "rumps", "rungs", "runny", "runty", "rural", "rushy", "rusts", "ruths", "rutty", "rykes", "ryots", "sabed", "sabes", "sabir", "sabot", "sabre", "sacra", "sadhe", "sadis", "safer", "sagas", "sages", "sagos", "sahib", "saids", "sails", "saint", "sajou", "sakes", "salad", "salep", "salic", "salmi", "salon", "salps", "salts", "salve", "samba", "samek", "sands", "saned", "sanes", "sangh", "sapid", "sappy", "sards", "sarge", "sarin", "sarks", "sarod", "sasin", "satay", "satem", "satin", "satyr", "sauch", "saugh", "sault", "saury", "saved", "saves", "savor", "savvy", "sawer", "sayed", "sayid", "scabs", "scags", "scale", "scalp", "scamp", "scans", "scape", "scarf", "scars", "scary", "scatt", "scaur", "scend", "scent", "schmo", "schwa", "scoff", "scone", "scoot", "scops", "scorn", "scour", "scowl", "scrag", "scrap", "screw", "scrip", "scrub", "scuba", "scudo", "scuff", "scull", "scums", "scurf", "scute", "scuzz", "seams", "sears", "sebum", "sects", "seder", "sedgy", "seeds", "seeks", "seely", "seeps", "seers", "segno", "segue", "seine", "seism", "selah", "selle", "selva", "semes", "sends", "senna", "sensa", "sente", "sepal", "sepic", "septa", "serac", "seral", "serer", "serfs", "serif", "serow", "serum", "servo", "setal", "setts", "seven", "sewan", "sewed", "sexed", "sexto", "shack", "shads", "shaft", "shahs", "shako", "shale", "shalt", "shame", "shank", "shard", "shark", "sharp", "shave", "shawm", "shaws", "sheaf", "shear", "sheds", "sheep", "sheet", "shelf", "shend", "sheol", "shewn", "shied", "shier", "shift", "shily", "shine", "shiny", "shire", "shirr", "shist", "shiva", "shivs", "shlub", "shoat", "shoed", "shoes", "shogs", "shone", "shool", "shoos", "shops", "shorl", "short", "shots", "shout", "shown", "showy", "shred", "shris", "shrug", "shuck", "shuls", "shunt", "shute", "shwas", "shyly", "sibbs", "sices", "sicks", "sides", "sidle", "sieur", "sifts", "sight", "sigla", "signa", "sikas", "sikes", "silex", "silky", "silly", "silts", "silva", "simas", "since", "sinew", "sings", "sinks", "siped", "sired", "siren", "sirra", "sisal", "sissy", "sited", "situp", "siver", "sixmo", "sixth", "sizar", "sizer", "skags", "skank", "skats", "skeed", "skees", "skegs", "skell", "skelp", "skeps", "skids", "skier", "skiey", "skill", "skimp", "skink", "skint", "skirl", "skirt", "skits", "skoal", "skosh", "skulk", "skunk", "skyey", "slack", "slain", "slams", "slank", "slaps", "slate", "slaty", "slaws", "sleds", "sleep", "slept", "slice", "slide", "slily", "slims", "sling", "slipe", "slipt", "slobs", "slogs", "slojd", "slope", "slosh", "slots", "sloyd", "slued", "sluff", "slump", "slung", "slurb", "slurs", "sluts", "slyly", "smack", "smalt", "smart", "smaze", "smeek", "smelt", "smews", "smirk", "smith", "smogs", "smoky", "smote", "smuts", "snafu", "snail", "snaky", "snare", "snark", "snash", "snaws", "sneap", "sneds", "snell", "snick", "sniff", "snips", "snobs", "snood", "snool", "snoot", "snort", "snout", "snowy", "snuck", "snugs", "soaks", "soapy", "soave", "sober", "socko", "socle", "soddy", "sodom", "sofas", "softs", "soggy", "sojas", "sokol", "solar", "soldo", "solei", "solid", "solos", "solus", "soman", "sonar", "sones", "sonic", "sonny", "sooey", "sooth", "sooty", "sophy", "soppy", "sorbs", "sored", "sorer", "sorgo", "sorry", "sorts", "soths", "sough", "souls", "soups", "sours", "south", "sowed", "soyas", "sozin", "spacy", "spado", "spaes", "spail", "spake", "spall", "spang", "spans", "spark", "spasm", "spats", "spays", "speak", "spear", "specs", "speel", "speil", "spell", "spend", "sperm", "spica", "spick", "spicy", "spiel", "spies", "spike", "spiky", "spill", "spine", "spiny", "spirt", "spite", "spitz", "splat", "split", "spoil", "spoof", "spool", "spoor", "sport", "spout", "sprat", "spree", "sprit", "sprug", "spued", "spume", "spunk", "spurs", "sputa", "squad", "squaw", "squib", "stabs", "stade", "stage", "stagy", "staig", "stair", "stale", "stall", "stand", "stang", "staph", "stark", "start", "state", "stave", "stead", "steal", "steed", "steel", "steer", "stela", "stems", "stent", "stere", "stets", "stewy", "stick", "sties", "stile", "stilt", "stimy", "stink", "stipe", "stirp", "stoae", "stoas", "stobs", "stogy", "stoke", "stoma", "stone", "stood", "stool", "stope", "stopt", "stork", "story", "stots", "stoup", "stout", "stowp", "strap", "stray", "strew", "strip", "strow", "strum", "stubs", "studs", "stuff", "stump", "stung", "stuns", "stupa", "sturt", "styes", "styli", "suave", "subas", "sucks", "sucre", "sudor", "suede", "suets", "sugar", "suing", "suite", "sulci", "sulfo", "sulky", "sulus", "summa", "sumps", "sunns", "sunup", "supes", "surah", "suras", "surer", "surfy", "surgy", "surra", "sutra", "swabs", "swags", "swain", "swami", "swamy", "swank", "swaps", "sware", "swarm", "swash", "swats", "swear", "swede", "sweer", "swell", "swift", "swill", "swine", "swink", "swirl", "swiss", "swive", "swoon", "swops", "swore", "swots", "swung", "syces", "sylis", "sylva", "syncs", "synth", "syren", "sysop", "taber", "tabid", "table", "tabor", "tabus", "tacet", "tachs", "tacks", "tacos", "taels", "tafia", "taiga", "tains", "tajes", "taken", "takes", "talar", "talcs", "tales", "talky", "tally", "taluk", "tamal", "tamer", "tamis", "tamps", "tango", "tangy", "tanks", "tanto", "taped", "tapes", "tapis", "tardy", "tares", "tarns", "tarok", "tarot", "tarre", "tarsi", "tarty", "tasse", "tasty", "tater", "tatty", "tauon", "tauts", "tawer", "tawny", "taxed", "taxes", "taxol", "taxus", "tazze", "teaks", "teams", "teary", "teats", "techy", "teddy", "teems", "teeny", "teffs", "tegua", "teind", "telco", "telex", "telic", "telly", "telos", "teemo", "tempo", "tempt", "tends", "tenet", "tenia", "tenor", "tenth", "tenty", "tepas", "tepid", "terai", "terga", "terne", "terra", "terse", "testa", "testy", "tetra", "teuch", "tewed", "texts", "thane", "tharm", "thebe", "theft", "thein", "theme", "there", "these", "theta", "thewy", "thief", "thill", "thing", "thins", "third", "thole", "thorn", "thorp", "thous", "three", "thrip", "throe", "thrum", "thugs", "thumb", "thunk", "thuya", "thymi", "tiara", "tical", "tidal", "tides", "tiffs", "tight", "tikes", "tikka", "tilde", "tiler", "tills", "tilts", "timer", "timid", "tinea", "tines", "tings", "tints", "tippy", "tired", "tirls", "titan", "tithe", "title", "titty", "toads", "toast", "toddy", "toffs", "tofts", "togae", "togue", "toils", "tokay", "token", "tokes", "tolar", "toled", "tolls", "tolyl", "tombs", "tommy", "tondi", "toned", "tones", "tonga", "tonic", "tonus", "toons", "toots", "toped", "toper", "tophe", "tophs", "topis", "topos", "torah", "torch", "tores", "torii", "torot", "torse", "torsk", "torta", "torts", "total", "totem", "totes", "tough", "touse", "towed", "tower", "towns", "toxic", "toyed", "toyon", "trace", "tract", "tragi", "trail", "trait", "trams", "tranq", "traps", "trash", "trave", "trays", "treat", "treen", "treks", "tress", "trews", "triac", "trial", "trice", "tried", "tries", "trigs", "trill", "trine", "trios", "trips", "troak", "trode", "trois", "troll", "trona", "troop", "trope", "trots", "trove", "troys", "truck", "truer", "trugs", "truly", "trunk", "trust", "tryma", "tsade", "tsars", "tsuba", "tubal", "tubby", "tuber", "tucks", "tuffs", "tufty", "tulip", "tumid", "tumor", "tunas", "tuner", "tungs", "tunny", "tuque", "turds", "turfy", "turns", "tushy", "tutee", "tutti", "tutus", "tuyer", "twain", "twats", "tweed", "tweet", "twice", "twigs", "twine", "twiny", "twirp", "twits", "twyer", "tyers", "tyiyn", "tyned", "typal", "types", "typic", "typps", "tyres", "tythe", "udder", "uhlan", "ulama", "ulcer", "ulnad", "ulnar", "ulpan", "ulvas", "umbel", "umbos", "umiac", "umiaq", "unais", "unarm", "unaus", "unbar", "unbox", "uncia", "uncos", "uncus", "undee", "undid", "unfed", "unfix", "unhat", "unify", "unite", "unity", "unlay", "unlet", "unman", "unmew", "unpeg", "unpin", "unrip", "unset", "unsex", "until", "unwet", "unwon", "upbow", "updos", "upend", "upped", "upset", "urare", "urase", "urban", "ureal", "uredo", "urged", "urges", "urine", "ursae", "usage", "usher", "usnea", "usual", "usury", "utile", "uveal", "uvula", "vagal", "vagus", "vairs", "vales", "valid", "valse", "valve", "vampy", "vaned", "vangs", "vapor", "varia", "varna", "varve" };
string former_name = "";
mt19937 generator(int(time(nullptr)));
uniform_real_distribution<double> distribution(1, 4700);
void random_name(string &placeholder);
int recv_data(string &type_h, string &data_h);
thread client_handler;

struct Connection {
	SOCKET active_socket;
	int socket_number;
	string unique_name;
	string ip;
};

vector<Connection> m_connect;
Connection active;
Connection m_new_connection(SOCKET sock, int sock_num, string unique_name, string ip);
Connection find_struct(vector<Connection> m, string unq_nam);
Connection find_struct(vector<Connection> m, int sc_num);
Connection find_by_ip(vector<Connection> s, string ip);
int find_by_num(vector<Connection> ms, int num);
bool verbose = true;
bool check_error(int status);
void connection_handler(fd_set &connection_master, int port, SOCKET &listening);
void commandline();
void connect(string command);
void disconnect(string command);
void send_message(string message);
void list(string command);
void help(string command);
void settings(string command);
void detach();
bool send_msg(string message, string header);
void exit();
void disp_connection(Connection c);
void ShowCursor(bool show);
void clear();
void display_banner();
void start(string type);
int clear_count = 0;
int auto_clear_thrs = 3;
void execute_cmd(string command);
void execute_ps(string command);
void execute_nir(string command);
bool cmp_unq(Connection c, string unq_name);
bool is_socket_num(string input);
bool is_unique(string input);
bool is_digit(string input);
bool is_alpha(string input);
bool is_ip(string input);
bool is_connection(Connection c);
bool cmp_socknum(Connection c, int socknum);
bool cmp_ip(Connection c, string ip);
bool connected = false;

template<typename paramType>
map<string, function<void(paramType)>> function_standard = {
	{"list", [](paramType x) {list(x); }},
	{"connect", [](paramType x) {connect(x); }},
	{"disconnect", [](paramType x) {disconnect(x); }},
	{"clear", [](paramType x) {clear(); }},
	{"help",[](paramType x) {help(x); }},
	{"settings", [](paramType x) {settings(x); }},
	{"exit", [](paramType x) {exit(); }}
};

template<typename paramType>
map<string, function<void(paramType)>> function_connected = {
	{"detach", [](paramType x) {detach(); }},
	{"tsend", [](paramType x) {send_message(x); }},
	{"cmd",[](paramType x) {execute_cmd(x); }},
	{"ps", [](paramType x) {execute_ps(x); }},
	{"nircmd", [](paramType x) {execute_nir(x); }},
	{"start", [](paramType x) {start(x); }}
};

int main()
{
	clear();
	client_handler = thread(connection_handler, ref(global_set), 1337, ref(listening_socket));
	client_handler.detach();
	display_banner();
	while (true) {
		commandline();
	}
}

bool check_error(int status) {
	if (status == SOCKET_ERROR) {
		wchar_t *s = NULL;
		FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, WSAGetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPWSTR)&s, 0, NULL);
		if (verbose) {
			wcout << "error: " << WSAGetLastError() << " | " << s << endl;
		}

		LocalFree(s);
		return false;
	}
	else {
		return true;
	}
}

void display_banner() {
	vector<string> banner = {
	R"(        _/_/_/  _/        _/_/_/      _/_/_/      _/_/    _/_/_/_/_/   )",
	R"(     _/        _/          _/        _/    _/  _/    _/      _/        )",
	R"(    _/        _/          _/        _/_/_/    _/_/_/_/      _/         )",
	R"(   _/        _/          _/        _/    _/  _/    _/      _/          )",
	R"(    _/_/_/  _/_/_/_/  _/_/_/      _/    _/  _/    _/      _/           )"
	};
	SetCaretBlinkTime(500);
	cout << endl;
	for (auto i : banner) {
		for (auto c : i) {
			cout << c;
			Sleep(3);
		}
		cout << endl;
	}
	cout << endl;
}

void settings(string command) {
	/*
	enter 13
	up 72
	down 80
	left 75
	right 77
	esc 27
	*/
	while (1) {
		if (_kbhit()) {
			auto key_code = _getch();
			cout << key_code << endl;
		}
		else {
			continue;
		}
	}
}

COORD GetConsoleCursorPosition(HANDLE hConsoleOutput)
{
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
	{
		return cbsi.dwCursorPosition;
	}
	else
	{
		// The function failed. Call GetLastError() for details.
		COORD invalid = { 0, 0 };
		return invalid;
	}
}

void ShowCursor(bool show) {
	HANDLE hConsoleOutput;
	CONSOLE_CURSOR_INFO structCursorInfo;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleCursorInfo(hConsoleOutput, &structCursorInfo);
	structCursorInfo.bVisible = show;
	SetConsoleCursorInfo(hConsoleOutput,
		&structCursorInfo);
}

void execute_cmd(string command) {
	send_msg(command, "CMDO");
	string type, data;
	cout << "cmd.exe output for \"" << command << "\"" << endl;
	while (type != "CMDE") {
		recv_data(type, data);
		if (!(data == "\n" || data == "\r")) {
			cout << data;
		}
		Sleep(10);
	}
	cout << endl;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = GetConsoleCursorPosition(h);
	c.Y -= 1;
	SetConsoleCursorPosition(h, c); // this little hack remove the current directory from outputing after the commadn ouput
}

void execute_ps(string command)
{
	send_msg(command, "POSO");
	string type, data;
	cout << "powershell.exe output for \"" << command << "\"" << endl;
	while (type != "POSE") {
		recv_data(type, data);
		if (!(data == "\n" || data == "\r")) {
			cout << data;
		}
		Sleep(100);
	}
	cout << endl;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = GetConsoleCursorPosition(h);
	c.Y -= 1;
	SetConsoleCursorPosition(h, c); // this little hack remove the current directory from outputing after the command ouput
}

void execute_nir(string command)
{
	send_msg(command, "NIRO");
	cout << "Nircmd.dll return for \"" << command << "\" = ";
	string data, type;
	recv_data(type, data);
	if (type == "NIRB") {
		cout << "(" << data << ")" << endl;
	}
}

void detach()
{
	prefix = default_prefix;
	connected = false;
	Connection default_connection;
	active = default_connection;
}

int recv_data(string &type_h, string &data_h) {
	char type[5]; // 4 + 1('\0')
	char size[9]; // 8 + 1('\0')
	int alloc_size;
	ZeroMemory(type, 5);
	ZeroMemory(size, 9);
	if (check_error(recv(active.active_socket, type, 4, 0))) {
		type_h = type;
		if (check_error(recv(active.active_socket, size, 8, 0))) {
			alloc_size = atoi(size);
			char *input = new char[alloc_size];
			if (check_error(recv(active.active_socket, input, alloc_size, 0))) {
				input[alloc_size] = '\0';
				data_h = input;
			}
			else {
				return -1;
			}
		}
		else {
			return -2;
		}
	}
}

bool send_msg(string input, string header) {
	int msg_size = input.size();
	int padding = msg_size > 0 ? (int)log10((double)msg_size) + 1 : 1;
	stringstream ss;
	ss << string(8 - padding, '0') << msg_size;
	string data_size = ss.str();
	string type = header;
	string data = input;
	if (!(check_error(send(active.active_socket, type.c_str(), type.size(), 0)) && check_error(send(active.active_socket, data_size.c_str(), data_size.size(), 0)) && check_error(send(active.active_socket, data.c_str(), data.size(), 0)))) {
		return false;
	}
	else {
		return true;
	}
}

void send_message(string message) {
	send_msg(message, "TEXT");
	string data, type;
	recv_data(type, data);
}

void disconnect(string command) {
	if (command == "-l") {
		Connection subject;
		if (!m_connect.empty()) {
			subject = m_connect.back();
			if (is_connection(subject)) {
				closesocket(subject.active_socket);
				m_connect.erase(m_connect.begin() + find_by_num(m_connect, subject.socket_number));
				if (verbose) {
					cout << "Disconnected Socket (" << subject.socket_number << ")" << endl;
				}
			}
		}
		else {
			if (verbose) {
				cout << "error: there is no active connections to the server" << endl;
			}

			return;
		}
	}
	else if (is_unique(command)) {
		Connection subject = find_struct(m_connect, command);
		if (is_connection(subject)) {
			closesocket(subject.active_socket);
			m_connect.erase(m_connect.begin() + find_by_num(m_connect, subject.socket_number));
			if (verbose) {
				cout << "Disconnected (" << command << ")" << endl;
			}
		}
		else {
			return;
		}
	}
	else if (is_socket_num(command)) {
		int socket_number = stoi(command);
		Connection subject = find_struct(m_connect, socket_number);
		if (is_connection(subject)) {
			closesocket(subject.active_socket);
			m_connect.erase(m_connect.begin() + find_by_num(m_connect, subject.socket_number));
			if (verbose) {
				cout << "Disconnected (" << command << ")" << endl;
			}
		}
		else {
			return;
		}
	}
	else if (is_ip(command)) {
		Connection subject = find_by_ip(m_connect, command);
		if (is_connection(subject)) {
			closesocket(subject.active_socket);
			m_connect.erase(m_connect.begin() + find_by_num(m_connect, subject.socket_number));
			if (verbose) {
				cout << "Disconnected (" << command << ")" << endl;
			}
		}
		else {
			return;
		}
	}
	else {
		cout << "syntax: disconnect [<unique_name>, <socket_id>, <ip>]" << endl;
	}
}

void connect(string command) {
	if (command == "-l") {
		Connection subject = m_connect.back();
		if (is_connection(subject)) {
			stringstream pre;
			pre << char(tolower(subject.unique_name[0])) << subject.unique_name.substr(1) << "@" << "client";
			prefix = pre.str();
			connected = true;
			active = subject;
			if (verbose) {
				cout << "Connected to [\"" << subject.unique_name << "\"] - > (" << subject.socket_number << ")" << endl;
			}
		}
	}
	else if (is_unique(command)) {
		Connection subject = find_struct(m_connect, command);
		if (is_connection(subject)) {
			stringstream pre;
			pre << char(tolower(subject.unique_name[0])) << subject.unique_name.substr(1) << "@" << "client";
			prefix = pre.str();
			connected = true;
			active = subject;
			if (verbose) {
				cout << "Connected to [\"" << subject.unique_name << "\"] - > (" << subject.socket_number << ")" << endl;
			}
		}
		else {
			return;
		}
	}
	else if (is_socket_num(command)) {
		int socket_number = stoi(command);
		Connection subject = find_struct(m_connect, socket_number);
		if (is_connection(subject)) {
			stringstream pre;
			pre << char(tolower(subject.unique_name[0])) << subject.unique_name.substr(1) << "@" << "client";
			prefix = pre.str();
			connected = true;
			active = subject;
			if (verbose) {
				cout << "Connected to [\"" << subject.unique_name << "\"] - > (" << subject.socket_number << ")" << endl;
			}
		}
		else {
			return;
		}
	}
	else if (is_ip(command)) {
		Connection subject = find_by_ip(m_connect, command);
		if (is_connection(subject)) {
			stringstream pre;
			pre << char(tolower(subject.unique_name[0])) << subject.unique_name.substr(1) << "@" << "client";
			prefix = pre.str();
			connected = true;
			active = subject;
			if (verbose) {
				cout << "Connected to [\"" << subject.unique_name << "\"] - > (" << subject.socket_number << ")" << endl;
			}
		}
		else {
			return;
		}
	}
	else {
		cout << "syntax: connect [<unique_name>, <socket_id>, <ip>]" << endl;
	}
}

void list(string command) {
	if (is_digit(command)) {
		int upto = stoi(command);
		cout << "Showing (" << upto << ") client" << ((upto == 1) ? " " : "s ") << "{" << endl;
		for (int i = 0; i <= upto - 1; i++) {
			disp_connection(m_connect[i]);
		}
		cout << "}" << endl;
	}
	else if (command == "clients" || command == "connections" || command == "*") {
		for (auto i = m_connect.begin(); i != m_connect.end(); i++) {
			disp_connection(*i);
		}
	}
	else if (command == "#clients" || command == "#") {
		cout << "Number of connected clients are: [" << m_connect.size() << "]" << endl;
	}
	else {
		cout << "syntax: list [<clients/connections>, <#clients/#>] / number" << endl;
	}
}

bool is_digit(string input) {
	for (unsigned char c : input) {
		if (isdigit(c)) {
			continue;
		}
		else {
			return false;
		}
	}
	return true;
}

bool is_socket_num(string input) {
	if (!(input.length() != 3)) {
		for (unsigned char c : input) {
			if (isdigit(c)) {
				continue;
			}
			else {
				return false;
			}
		}
		return true;
	}
	else {
		return false;
	}
}

bool is_alpha(string input) {
	for (unsigned char c : input) {
		if (isalpha(c)) {
			continue;
		}
		else {
			return false;
		}
	}
	return true;
}

bool is_unique(string input) {
	return (is_alpha(input) && input.length() >= 4 && input.length() <= 5);
}

bool is_ip(string input) {
	int length = (int)input.length();
	int dot_count = 0;
	if (length >= 7 && length <= 15) {
		for (unsigned char c : input) {
			if (isdigit(c) || c == '.') {
				if (c == '.') {
					dot_count++;
				}
				continue;
			}
			else {
				return false;
			}
		}
		if (dot_count == 3) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

bool is_connection(Connection c) {
	return (c.ip != "" && c.socket_number != NULL && c.unique_name != "");
}

void commandline() {
	string input, parameter, function;
	cout << prefix << " ~ $ ";
	getline(cin, input);
	for (auto i : m_connect) {
		active = i;
		send_msg("ALIVE", "ALIV");
		string type, data;
		recv_data(type, data);
		if (!(type == "ALIV" && data == "OK")) {
			disconnect(active.unique_name);
		}
	}
	if (input.empty()) {
		clear_count++;
		if (clear_count >= auto_clear_thrs) {
			clear();
			clear_count = 0;
		}
		return;
	}
	auto command = input.find(" ");
	if (command != string::npos && command != 0) { //finds space between function and parameter
		clear_count = 0;
		function = input.substr(0, command);
		parameter = input.substr(command + 1);
	}
	else { // assumes to only call function without parameter
		clear_count = 0;
		function = input.substr(0, command);
		parameter = " ";
	}

	map<string, std::function<void(decltype(function))>> active_map;
	if (connected) {
		active_map = function_connected<decltype(function)>;
	}
	else {
		active_map = function_standard<decltype(function)>;
	}
	if (active_map.find(function) != active_map.end()) {
		active_map[function](parameter);
	}
	else {
		cout << "'" << function << "' is not a recognized command. ";
		string query;
		vector<string> potential_string;
		vector<int> potestial_int;
		for (auto c : function) {
			query += c;
			int count = 0;
			for (auto f : active_map) {
				if (f.first.find(query) != string::npos) {
					count++;
					potential_string.push_back(f.first);
				}
			}
			if (count > 0) {
				potestial_int.push_back(count);
			}
		}
		if (!(potential_string.empty()) && !(potestial_int.empty())) {
			cout << "Did you mean [";
			for (int i = 1; i <= potestial_int.back(); i++) {
				cout << "\"" << potential_string[potential_string.size() - i] << "\"" << " or ";
			}
			cout << "\b\b\b\b] ?" << endl;
		}
		else {
			cout << endl;
		}
	}
}

void help(string command) {
	map<string, string> help_tree{
	{"help", "Shows documentation for specific function/Show all available functions"},
	{"list", "Shows all current connection and thier unique name"},
	{"connect", "Connects to a specific client with unique name or number"},
	{"disconnect", "Disconnects/kicks client from server"},
	{"settings", "Changes server settings"},
	{"restart", "Tries to restart server"},
	{"clear", "Clears and resets screen"},
	{"exit", "Exits the server and disconnects all clients"},
	};

	if (command == " " || command == "") {
		stringstream first;
		first << " -- Available Commands --";
		cout << first.str() << endl;
		for (auto i : function_standard<decltype(command)>) {
			stringstream ss;
			ss << "| " << i.first;
			cout << ss.str() << string(first.str().length() - ss.str().length(), ' ') << "|" << endl;
		}
		cout << " " << string(first.str().length() - 1, '-') << endl;
		cout << " Specific='help command'" << endl
			<< " All commands='help *'" << endl;
	}
	else if (command.find("*") != string::npos || command == "all") {
		cout << "The Help File For all the Commands {" << endl;
		for (auto i : help_tree) {
			cout << "  ('" << i.first << " - " << i.second << "');" << endl;
		}
		cout << "}" << endl;
	}
	else {
		string documentation;
		auto search = help_tree.find(command);
		if (search != help_tree.end()) {
			documentation = search->second;
			cout << "The Help File For Commmand ('" << command << "') {" << endl
				<< "  ('" << command << " - " << documentation << "');" << endl << "}" << endl;
		}
		else {
			cout << "Error: Could not find help file of the command '" << command << "'" << endl;
		}
	}
}

void start(string type) {
	if (type == "cmd") {
		send_msg(" ", "CMDS");
		string start_t, start_d;
		while (start_t != "CMDE") {
			recv_data(start_t, start_d);
			cout << start_d;
		}

		while (true) {
			string output_l, type_l;
			cout << start_d << "\b";
			string command;
			getline(cin, command);
			if (command == "exit") {
				send_msg(command, "CMDC");
				send_msg(" ", "CMDK");
				break;
			}
			else if (command.empty()) {
				send_msg(" ", "CMDC");
			}
			else {
				send_msg(command, "CMDC");
			}
			while (type_l != "CMDE") {
				recv_data(type_l, output_l);
				cout << output_l;
			}
		}
	}
}

void exit() {
	exit(0);
}

void random_name(string & name)
{
	while (name.empty()) {
		string wordL = randomWords[int(distribution(generator))];
		if (!wordL.empty()) {
			try {
				name = char(toupper(wordL[0])) + wordL.substr(1);
			}
			catch (std::exception e) {
				cout << "internal: " << e.what() << endl;
			}
		}
		if (name == former_name) {
			wordL = randomWords[int(distribution(generator))];
			if (!wordL.empty()) {
				try {
					name = char(toupper(wordL[0])) + wordL.substr(1);
				}
				catch (std::exception e) {
					cout << "internal: " << e.what() << endl;
				}
			}
		}
		former_name = name;
	}
}

void clear() {
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
}

void disp_connection(Connection c) {
	stringstream fr;
	stringstream sr;
	stringstream ss;

	fr << "| [Unique Name: \"" << c.unique_name << "\"]";
	sr << "| [Socket ID: " << c.socket_number << "]";
	ss << "| [Protocol Adress: " << c.ip << "]  |";

	string frst = fr.str();
	string sec = sr.str();
	string thrd = ss.str();

	cout << string(thrd.length(), '-'); // string(n, 'c') print c n-times (only chars no strings)
	cout << endl << frst << string(thrd.length() - frst.length() - 1, ' ') << "|" << endl << ss.str() << endl;
	cout << sr.str() << string(thrd.length() - sec.length() - 1, ' ') << "|" << endl;
	cout << string(thrd.length(), '-') << endl;
}

Connection m_new_connection(SOCKET sock, int sock_num, string unique_name, string ip) {
	Connection c;
	c.active_socket = sock;
	c.socket_number = sock_num;
	c.unique_name = unique_name;
	c.ip = ip;
	return c;
}
//https://thispointer.com/using-stdfind-stdfind_if-with-user-defined-classes/
Connection find_struct(vector<Connection> m, string unq_nam) {
	vector<Connection>::iterator it;
	it = find_if(m.begin(), m.end(), std::bind(cmp_unq, placeholders::_1, unq_nam));
	if (it != m.end()) {
		stringstream ss;
		if (verbose) {
			cout << "Found connection [" << it->socket_number << "] with name \"" << unq_nam << "\"" << endl;
		}

		return *it;
	}
	else {
		if (verbose) {
			cout << "syntax: could not find a connection with the name \"" << unq_nam << "\"";
		}

		Connection er;
		return er;
	}
}

Connection find_struct(vector<Connection> ms, int sc_num) {
	vector<Connection>::iterator it;
	it = find_if(ms.begin(), ms.end(), std::bind(cmp_socknum, placeholders::_1, sc_num));
	if (it != ms.end()) {
		if (verbose) {
			cout << "Found \"" << it->unique_name << "\" connection with number [" << sc_num << "]" << endl;
		}

		return *it;
	}
	else {
		if (verbose) {
			cout << "syntax: could not find a connection with the number " << sc_num << endl;
		}

		Connection er;
		return er;
	}
}

Connection find_by_ip(vector<Connection> s, string ip) {
	vector<Connection>::iterator it;
	it = find_if(s.begin(), s.end(), std::bind(cmp_ip, placeholders::_1, ip));
	if (it != s.end()) {
		if (verbose) {
			cout << "Found \"" << it->unique_name << "\" connection with ip \"" << ip << "\"" << endl;
		}

		return *it;
	}
	else {
		if (verbose) {
			cout << "syntax: could not find a connection with the ip " << ip << endl;
		}

		Connection r;
		return r;
	}
}

int find_by_num(vector<Connection> ms, int num) {
	vector<Connection>::iterator it;
	it = find_if(ms.begin(), ms.end(), std::bind(cmp_socknum, placeholders::_1, num));
	if (it != ms.end()) {
		int index = it - ms.begin();
		return index;
	}
	else {
		return -22;
	}
}

bool cmp_unq(Connection c, string unq_name) {
	if (c.unique_name == unq_name) {
		return true;
	}
	else {
		return false;
	}
}

bool cmp_socknum(Connection c, int socknum) {
	if (c.socket_number == socknum) {
		return true;
	}
	else {
		return false;
	}
}

bool cmp_ip(Connection c, string ip) {
	if (c.ip == ip) {
		return true;
	}
	else {
		return false;
	}
}

void auth(int socket_id) {
	stringstream auth_token;
	auth_token << "AUTH" << socket_id;
	send_msg(auth_token.str(), "AUTH");
	srand(socket_id);
	string phrase = "CDLXXII";
	int sum = 0;
	for (auto i : phrase) { sum += ((i ^ socket_id) ^ rand()); }
	string password = to_string(sum);
	string type, data;
	recv_data(type, data);
	if (!(type == "CERT" && data == password)) {
		cout << endl << "Unauthorized Access Made by socket (" << socket_id << ")" << endl << prefix << " ~ $ ";
		send_msg("FAULTY", "AUTF");
		closesocket(active.active_socket);
		m_connect.erase(m_connect.begin() + find_by_num(m_connect, active.socket_number));
	}
	else {
		send_msg("GRANTED", "AUTG");
	}
}

void connection_handler(fd_set &connection_master, int port, SOCKET &listening) {
	if (!check_error(WSAStartup(ver, &wsData))) {
		cerr << "Can't Initialize winsock! Quitting" << endl;
		return;
	}
	listening = socket(AF_INET, SOCK_STREAM, 0);
	if (!check_error(listening))
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return;
	}

	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;
	::bind(listening, (sockaddr*)&hint, sizeof(hint));
	listen(listening, SOMAXCONN);
	FD_ZERO(&connection_master);
	FD_SET(listening, &connection_master);
	//cout << "Server running with binding_port - " << port << endl;

	while (true)
	{
		timeval tv;
		tv.tv_sec = 5;
		tv.tv_usec = 1;
		fd_set copy = connection_master;
		auto socket_count = select(0, &copy, nullptr, nullptr, &tv);
		for (int i = 0; i < socket_count; i++)
		{
			SOCKET sock = copy.fd_array[i];
			if (sock == listening) //handle new connections BUG:[when having multiple clients reconnecting at the same time after server has closed the clients will get the same unique name]
			{					   //fixed by initilizing random name in varible and having seed be dependent on the time of startup. the problem was that the clients were reconnecting at the same time and that resulted in the seed being at the same millisecond and the resultet in the same unique name
				sockaddr_in clientad;
				int clientSize = sizeof(clientad);
				SOCKET client = accept(listening, (sockaddr*)&clientad, &clientSize);
				string unique_name;
				Connection default_connection;
				random_name(unique_name);
				FD_SET(client, &connection_master);
				char host[NI_MAXHOST];
				char service[NI_MAXSERV];

				ZeroMemory(host, NI_MAXHOST);
				ZeroMemory(service, NI_MAXSERV);

				if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
				{
					m_connect.push_back(m_new_connection(client, client, unique_name, host));
				}
				else
				{
					inet_ntop(AF_INET, &clientad.sin_addr, host, NI_MAXHOST);
					m_connect.push_back(m_new_connection(client, client, unique_name, host));
				}
				//accept then auth hardcoded password in client
				stringstream welcome_msg;
				string welcome;
				active = m_new_connection(client, client, unique_name, host);
				auth(client);
				active = default_connection;
			}
		}
	}

	FD_CLR(listening, &connection_master);
	closesocket(listening);
	WSACleanup();
}