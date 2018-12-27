<?php
/**
 * @file
 * Profanity filter.
 */

/**
 * Filter out profanity.
 *
 * @param string $message
 *   The message string to filter.
 *
 * @return string
 *   Returns 'XXX' if the string has profanity in it or the original message
 * string if no profanity is found.
 */
function zg_filter_profanity($message) {

  $profanity = [
	'4r5e',
	'5h1t',
	'5hit',
	'a_s_s',
	'a55',
	'anal',
	'anus',
	'ar5e',
	'arrse',
	'arse',
	'a§§',
	'ass',
	'asses',
	'assfucker',
	'ass-fucker',
	'assfukka',
	'asshole',
	'assholes',
	'asskisser',
	'asswhole',
	'asswipe',
        'asz',
        'azz',
	'beotch',
	'b!tch',
	'b00bs',
	'b17ch',
	'b1tch',
	'ballbag',
	'ballsack',
	'bastard',
	'beastial',
	'beastiality',
	'beastility',
	'beaver',
	'bellend',
	'belly whacker',
	'bestial',
	'bestiality',
	'beotch',
	'b!tch',
	'bytch',
	'bi+ch',
	'biatch',
	'bitch',
	'bitcher',
	'bitchers',
	'bitches',
	'bitchin',
	'bitching',
	'bloody',
	'blow job',
	'blowjob',
	'blowjobs',
	'boiolas',
	'bollock',
	'bollok',
	'bonehead',
	'boner',
	'boob',
	'boobs',
	'booobs',
	'boooobs',
	'booooobs',
	'booooooobs',
	'browntown',
	'btch',
	'buceta',
	'bucket cunt',
//	'bugger',
	'bullshit',
	'bung hole',
	'butch',
	'butt breath',
	'butt hair',
	'buttface',
	'buttfuck',
	'buttfucker',
	'butthead',
	'butthole',
	'buttmuch',
	'buttpicker',
	'buttplug',
	'c0ck',
	'c0cksucker',
	'carpet muncher',
	'cawk',
	'chink',
	'cipa',
	'circle jerk',
	'cl1t',
	'clam',
	'clit',
	'clitoris',
	'clits',
	'cnut',
	'cobia',
	'cock',
	'cockface',
	'cockhead',
	'cockmunch',
	'cockmuncher',
	'cocks',
	'cocksuck',
	'cocksucked',
	'cocksucker',
	'cock-sucker',
	'cocksucking',
	'cocksucks',
	'cocksuka',
	'cok',
	'cokmuncher',
	'coksucka',
	'coon',
	'cooter',
	'cox',
	'crap',
	'cum',
	'cummer',
	'cumming',
	'cums',
	'cumshot',
	'cunilingus',
	'cunillingus',
	'cunnilingus',
	'cunnilingus',
	'cunt',
	'cuntlick',
	'cuntlicker',
	'cuntlicking',
	'cunts',
	'cyalis',
	'cyberfuc',
	'cyberfuck',
	'cyberfucked',
	'cyberfucker',
	'cyberfuckers',
	'cyberfucking',
	'dammit',
	'd1ck',
	'damn',
	'damn',
	'dick',
	'dickk',
	'dickhead',
	'dike',
	'dildo',
	'dildos',
	'dink',
	'dinks',
	'dipshit',
	'dirsa',
	'dlck',
	'dog-fucker',
	'doggin',
	'dogging',
	'dong',
	'donkeyribber',
	'doosh',
	'douche bag',
	'duche',
	'dumbass',
	'dumbfuck',
	'dyke',
	'dyke',
	'ejaculate',
	'ejaculated',
	'ejaculates',
	'ejaculating',
	'ejaculatings',
	'ejaculation',
	'ejakulate',
	'fuc k',
	'fu c k',
	'fukin',
	'fck',
	'fvk',
	'f u c k',
	'f u c k e r',
	'f_u_c_k',
	'f4nny',
	'fag',
	'fagget',
	'fagging',
	'faggit',
	'faggitt',
	'faggot',
	'faggs',
	'fagot',
	'fagot',
	'fagots',
	'fags',
	'fanny',
	'fannyflaps',
	'fannyfucker',
	'fanyy',
	'fart',
	'farted',
	'farting',
	'fartings',
	'farts',
	'farty',
	'fatass',
	'fatso',
	'fcuk',
	'fcuker',
	'fcuking',
	'feck',
	'fecker',
	'felatio',
	'felching',
	'fellate',
	'fellatio',
	'fellatio',
	'fingerfuck',
	'fingerfucked',
	'fingerfucker',
	'fingerfuckers',
	'fingerfucking',
	'fingerfucks',
	'fistfuck',
	'fistfucked',
	'fistfucker',
	'fistfuckers',
	'fistfucking',
	'fistfuckings',
	'fistfucks',
	'flange',
	'fook',
	'fooker',
	'fuck',
	'fucka',
	'fucked',
	'fucker',
	'fuckers',
	'fuckers',
	'fuckhead',
	'fuckheads',
	'fuckin',
	'fucking',
	'fuckings',
	'fuckingshitmotherfucker',
	'fuckme',
	'fuckr',
	'fucks',
	'fuckwhit',
	'fuckwit',
	'fudge packer',
	'fudgepacker',
	'fuggin',
	'fuk',
	'fuker',
	'fukker',
	'fukkin',
	'fuks',
	'fukwhit',
	'fukwit',
	'furburger',
	'fux',
	'fux0r',
	'fvck',
	'gangbang',
	'gangbanged',
	'gangbangs',
	'gaylord',
	'gaysex',
	'gaysex',
	'gazongers',
	'goatse',
	'god-dam',
	'g--dam',
	'goddamn',
	'goddamn',
	'goddamned',
	'god-damned',
	'gonads',
	'gook',
	'guinne',
	'hard on',
	'hardcoresex',
	'hell',
	'heshe',
	'hoar',
	'hoare',
	'hoer',
	'homo',
	'hooker',
	'hore',
	'horniest',
	'horny',
	'hotsex',
	'hussy',
	'lmao',
	'lmfao',
	'jack off',
	'jackass',
	'jacking off',
	'jackoff',
	'jack-off',
	'jerk-off',
	'jism',
	'jiz',
	'jiz',
	'jizm',
	'jizz',
	'kawk',
	'kike',
	'knobead',
	'knobed',
	'knobend',
	'knobhead',
	'knobjocky',
	'knobjokey',
	'kock',
	'kondum',
	'kondums',
	'kum',
	'kummer',
	'kumming',
	'kums',
	'kunilingus',
	'kunilingus',
	'l3i+ch',
	'l3itch',
	'labia',
	'lesbian',
	'lesbo',
	'm0f0',
	'm0fo',
	'm45terbate',
	'ma5terb8',
	'ma5terbate',
	'masterb8',
	'masterbat*',
	'masterbat3',
	'masterbate',
	'master-bate',
	'masterbation',
	'masterbations',
	'masturbate',
	'mof0',
	'mofo',
	'mo-fo',
	'mothafuck',
	'mothafucka',
	'mothafuckas',
	'mothafuckaz',
	'mothafucked',
	'mothafucker',
	'mothafuckers',
	'mothafuckin',
	'mothafucking',
	'mothafuckings',
	'mothafucks',
	'mother fucker',
	'motherfuck',
	'motherfuck',
	'motherfucked',
	'motherfucker',
	'motherfuckers',
	'motherfuckin',
	'motherfuckin',
	'motherfucking',
	'motherfuckings',
	'motherfuckka',
	'motherfucks',
	'muff',
	'mutha',
	'muthafecker',
	'muthafuckker',
	'muther',
	'mutherfucker',
	'n1gga',
	'n1gger',
	'nazi',
	'nigg3r',
	'nigg4h',
	'nigga',
	'niggah',
	'niggas',
	'niggaz',
	'nigger',
	'niggers',
	'nobhead',
	'nobjocky',
	'nobjokey',
	'numbnuts',
	'nutsack',
	'orgasim',
	'orgasims',
	'orgasm',
	'orgasms',
	'p0rn',
	'pawn',
	'pecker',
	'penis',
	'penisfucker',
	'phonesex',
	'phuck',
	'phuk',
	'phuked',
	'phuking',
	'phuking',
	'phukked',
	'phukking',
	'phuks',
	'phuks',
	'phuq',
	'phuq',
	'pigfucker',
	'pimp',
	'pimpis',
	'piss',
	'pissed',
	'pisser',
	'pissers',
	'pisses',
	'pissflaps',
	'pissin',
	'pissing',
	'pissoff',
	'pissrr',
	'poop',
	'porn',
	'porno',
	'pornography',
	'pornos',
	'pron',
	'pube',
	'pusse',
	'pussi',
	'pussies',
	'pussies',
	'pussy',
	'pussys',
	'queer',
	'rectum',
	'retard',
	'rimjaw',
	'rimming',
	'rape',
	'shlt',
	'sh!t',
	'sh!ts',
	'slut',
	's hit',
	's.o.b.',
	's_h_i_t',
	'sadist',
	'schlong',
	'screw',
	'screwing',
	'scroat',
	'scrote',
	'scrotum',
	'semen',
	'sex',
	'sh!+',
	'sh!t',
	'sh1t',
	'shag',
	'shagger',
	'shaggin',
	'shagging',
	'sheister',
	'shemale',
	'shi+',
	'shit',
	'sh!t',
	'shitdick',
	'shite',
	'shited',
	'shited',
	'shitey',
	'shitfuck',
	'shitfull',
	'shitfull',
	'shithead',
	'shiting',
	'shitings',
	'shits',
	'shits',
	'shitted',
	'shitter',
	'shitters',
	'shitting',
	'shittings',
	'shitty',
	'shitty',
	'shiz',
	'sht',
	'shttim',
	'skank',
	'slag',
	'sleaze',
	'slut',
	'slut',
	'sluts',
	'smartass',
	'smegma',
	'smut',
	'snatch',
	'son-of-a-bitch',
	'spac',
	'spunk',
	'stupidass',
	't1tt1e5',
	't1tties',
	'teets',
	'teez',
	'testical',
	'testicle',
	'tit',
	'titfuck',
	'tits',
	'titt',
	'tittie5',
	'tittiefucker',
	'titties',
	'tittyfuck',
	'tittywank',
	'titwank',
	'tosser',
	'turd',
	'tw4t',
	'twat',
	'twathead',
	'twatty',
	'twunt',
	'twunter',
	'tampon',
	'v14gra',
	'v1gra',
	'vagina',
	'viagra',
	'vulva',
	'w00se',
	'wang',
	'wank',
	'wanker',
	'wanky',
	'wetback',
	'whoar',
	'whore',
	];

  $msg = str_replace(
    ['@', '$', "'", '.', '&#039;', '&quot;'],
    ['a', 's', ' ', ' ', ' ', ' '],
    $message);

  setlocale(LC_ALL, 'en_US');
  $msg = preg_replace('/[^A-Za-z0-9 ]/', '',
    iconv('utf-8', 'ascii//TRANSLIT', $msg));

  foreach ($profanity as $word) {

    // Check word boundaries, ignoring case.
    $pattern = "/\b$word\b/i";

    if (preg_match($pattern, $msg)) {
      return "XXX: found $word XXX";
    }
  }

  return $message;
}