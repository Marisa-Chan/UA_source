#include "includes.h"
#include "nucleas.h"
#include "particle.h"
#include "utils.h"

#include <math.h>


const Nucleus::ClassDescr NC_STACK_particle::description("particle.class", &newinstance);


struct prtcl_att
{
    int16_t version;
    NC_STACK_particle::StartEnd accel;
    NC_STACK_particle::StartEnd magnify;
    int32_t collide;
    int32_t startSpeed;
    int32_t contextNumber;
    int32_t contextLifeTime;
    int32_t contextStartGen;
    int32_t contextStopGen;
    int32_t genRate;
    int32_t lifeTime;
    int32_t startSize;
    int32_t endSize;
    int32_t noise;
};

static float particleRandomTable[1024] =
{
    -1.0, -0.998028993607, 0.137834995985, 0.534349024296, -0.837260007858,
    0.304504007101, -0.426658987999, -0.646332025528, -0.39246699214, -0.915395021439,
    -0.0711710005999, -0.165478006005, -0.754130005836, -0.401226997375, -0.0511050000787,
    -0.530840992928, 0.698112010956, -0.686707019806, -0.757524013519, -0.0550069995224,
    -0.222616001964, 0.0764169991016, -0.842932999134, 0.351114988327, -0.682431995869,
    -0.347885012627, -0.703749001026, 0.450024992228, 0.0550439991057, 0.894650995731,
    -0.0531620010734, -0.863035976887, 0.703279972076, -0.278138011694, -0.972280025482,
    -0.752663016319, 0.772518992424, -0.99448299408, -0.465321004391, 0.11380700022,
    0.272228002548, 0.955506980419, -0.360973000526, 0.235649004579, -0.234894007444,
    -0.687054991722, 0.436747997999, -0.419090986252, 0.606365025043, -0.647746026516,
    0.338313013315, 0.876726984978, -0.860638976097, 0.849695026875, 0.933314025402,
    0.716920018196, 0.781547009945, -0.637867987156, 0.822686970234, 0.396887987852,
    0.693033993244, 0.354254990816, 0.951647996902, -0.758705019951, 0.963793992996,
    0.360594987869, -0.586826980114, -0.258100986481, -0.331885010004, -0.676518976688,
    -0.761159002781, -0.607589006424, 0.173570007086, -0.89949798584, -0.964684009552,
    -0.595583021641, -0.322124987841, 0.844761013985, 0.112209998071, -0.588366985321,
    -0.152134999633, 0.188040003181, -0.449088007212, -0.390089005232, -0.415250986814,
    0.728375017643, -0.536414027214, 0.797095000744, -0.88613897562, -0.245530992746,
    -0.125510007143, 0.975308001041, 0.549373984337, -0.66462802887, 0.769450008869,
    -0.353646993637, -0.535880029202, -0.594637989998, -0.59193700552, -0.122059002519,
    -0.0633250027895, -0.783707022667, 0.539245009422, 0.152959004045, 0.264782011509,
    -0.675969004631, 0.173767000437, 0.688175022602, 0.0182900000364, 0.4363720119,
    -0.105802997947, -0.959097981453, 0.808436989784, -0.261130005121, -0.0383169986308,
    -0.73128002882, -0.271079003811, 0.057744000107, -0.0630280002952, 0.0524209998548,
    0.0360770002007, 0.455942988396, 0.204192996025, -0.0652040019631, -0.600031971931,
    -0.637485980988, 0.0751880034804, -0.545242011547, 0.0320330001414, 0.760493993759,
    0.883431971073, -0.964017987251, 0.54161798954, 0.710079014301, 0.783123016357,
    -0.445739001036, -0.826155006886, 0.888387978077, -0.272073000669, 0.0875490009785,
    0.0294269993901, 0.243698999286, -0.603298008442, 0.241298004985, -0.147236004472,
    0.00583600020036, 0.207570001483, -0.57825499773, 0.886192023754, -0.365121006966,
    0.607061982155, -0.903735995293, -0.218189999461, -0.930656015873, 0.0827699974179,
    0.725121021271, 0.294164985418, 0.182812005281, 0.326703995466, 0.176831007004,
    -0.527242004871, 0.0782879963517, 0.834922015667, -0.754526019096, -0.679539978504,
    -0.48101401329, 0.662239015102, -0.315405011177, 0.0515730008483, 0.22695299983,
    -0.282375007868, 0.471314013004, -0.745249986649, 0.834860980511, -0.242280006409,
    0.506644010544, 0.0466339997947, 0.0337960012257, 0.0881469994783, 0.498717010021,
    0.861927986145, -0.903091013432, -0.437900990248, -0.995616972446, 0.286377996206,
    0.455561995506, 0.0691049993038, 0.849808990955, 0.374074012041, -0.15268599987,
    -0.555661976337, -0.304688006639, -0.585806012154, 0.652000010014, 0.0498050004244,
    0.13228200376, 0.961371004581, -0.592575013638, 0.920486986637, 0.0920310020447,
    0.892400979996, -0.277144014835, -0.735112011433, 0.0238969996572, 0.89054197073,
    0.329607993364, 0.483319997787, -0.240704998374, -0.415726006031, -0.92114597559,
    -0.209923997521, 0.0339169986546, 0.718629002571, -0.740234017372, 0.0629720017314,
    -0.108464002609, -0.395195007324, -0.917429983616, -0.954535007477, 0.420477986336,
    0.65508300066, -0.686607003212, 0.363631010056, -0.0591339990497, -0.0623450018466,
    -0.532944977283, 0.622120976448, -0.509109020233, -0.261534005404, -0.979506015778,
    0.768413007259, -0.463317990303, 0.577174007893, -0.697084009647, 0.378343999386,
    0.574975013733, 0.593091011047, 0.199037000537, -0.791539013386, -0.00536599988118,
    -0.998526990414, 0.409435987473, -0.302742987871, 0.480203986168, -0.267315000296,
    -0.422338992357, 0.443529993296, 0.767560005188, -0.198754996061, 0.491239994764,
    -0.428389012814, 0.423839986324, -0.732968986034, -0.0330870002508, -0.994005978107,
    -0.113095000386, -0.822081983089, -0.718271970749, -0.630756020546, 0.922568976879,
    -0.82392001152, -0.576861023903, -0.144222006202, 0.656804025173, 0.232564002275,
    -0.00979600008577, -0.249042004347, 0.207449004054, -0.83111000061, -0.277220994234,
    0.290560990572, -0.0190919991583, -0.83448600769, 0.889292001724, -0.43885999918,
    -0.78677302599, -0.764505982399, -0.322632014751, 0.62394797802, 0.354925006628,
    -0.759283006191, -0.531476974487, -0.0108110001311, 0.930621981621, 0.956265985966,
    -0.0440540015697, 0.383421987295, 0.566524982452, 0.824123978615, -0.0394660010934,
    -0.705020010471, -0.00273300008848, -0.0622269995511, -0.281747996807, -0.729523003101,
    -0.0855560004711, 0.449310004711, -0.224535003304, -0.0317340008914, 0.733424007893,
    -0.675309002399, -0.531732976437, 0.915398001671, -0.83595597744, 0.274958997965,
    0.374752014875, -0.502839028835, -0.0275539997965, -0.0840789973736, 0.363521009684,
    0.202265992761, -0.699308991432, 0.496022999287, -0.438677996397, 0.895672023296,
    -0.707229971886, -0.18563400209, -0.641043007374, 0.821810007095, 0.578736007214,
    0.789762973785, -0.672945022583, 0.195938006043, -0.58403301239, 0.241867005825,
    -0.21201300621, 0.923655986786, 0.933740973473, -0.00361100002192, 0.889237999916,
    -0.64939302206, -0.833849012852, 0.873767971992, 0.148552000523, 0.707046985626,
    -0.623318016529, -0.652869999409, -0.156177997589, -0.584244012833, -0.61489200592,
    -0.143613994122, 0.413902014494, -0.812004983425, -0.71311300993, 0.684935986996,
    -0.0820690020919, -0.577066004276, -0.0934109985828, -0.98367702961, -0.511179983616,
    -0.827997028828, 0.342615008354, -0.0523019991815, 0.478655993938, -0.50227701664,
    -0.59719401598, 0.881844997406, -0.873921990395, 0.60139799118, -0.995770990849,
    -0.508282005787, -0.138474002481, 0.671177983284, 0.353242993355, 0.759666979313,
    -0.89891397953, 0.731859028339, -0.753324985504, 0.60275799036, 0.673866987228,
    -0.40838098526, -0.124814003706, 0.104034997523, -0.617884993553, 0.301939994097,
    0.0159889999777, 0.562474012375, 0.320486009121, -0.412995010614, 0.0407259985805,
    -0.485549002886, -0.965367972851, 0.562602996826, 0.815647006035, 0.275750994682,
    0.936372995377, 0.909700989723, 0.0618969984353, -0.672994017601, 0.0393019989133,
    0.987962007523, 0.623618006706, -0.940514028072, 0.559372007847, 0.0833050012589,
    0.694298028946, 0.548847973347, -0.659726977348, 0.569388985634, -0.0286379996687,
    -0.109851002693, -0.965795993805, -0.00476300017908, -0.187242001295, -0.376322001219,
    0.889527022839, -0.325062006712, 0.706085026264, 0.786916971207, 0.212720006704,
    0.726014971733, 0.54532200098, -0.27919998765, 0.997058987617, 0.561120986938,
    -0.700172007084, 0.598704993725, -0.267257988453, 0.479910999537, -0.231683000922,
    -0.71570199728, 0.485890001059, 0.809276998043, -0.963755011559, -0.541643023491,
    0.384122014046, 0.713959991932, -0.609673023224, -0.784285008907, 0.0674830004573,
    0.296566009521, -0.752986013889, -0.739210009575, 0.481195986271, -0.220294997096,
    0.271382004023, -0.521884977818, 0.148794993758, 0.938185989857, 0.323381006718,
    0.316554009914, -0.366932988167, -0.659914016724, 0.931327998638, -0.426845014095,
    -0.661648988724, -0.323579013348, -0.978362977505, 0.406971991062, 0.342002987862,
    -0.430343002081, 0.489895999432, -0.225759997964, 0.676241993904, 0.99071598053,
    -0.404639005661, -0.245715007186, 0.374451994896, -0.897013008595, -0.350930988789,
    -0.92991399765, -0.0578450001776, 0.421142995358, 0.339437007904, -0.514008998871,
    0.582801997662, -0.438643991947, -0.354483991861, 0.284666001797, -0.320942014456,
    -0.592139005661, -0.211948007345, -0.832708001137, 0.899483978748, 0.936487019062,
    -0.0640729963779, 0.539394974709, 0.761638998985, 0.699593007565, -0.298620998859,
    0.248998999596, -0.190657004714, 0.0972450003028, 0.971778988838, 0.246085003018,
    0.0376610010862, 0.143263995647, -0.493021011353, -0.0150439999998, 0.313075989485,
    -0.151825994253, 0.425247013569, 0.173897996545, 0.238011002541, 0.457969993353,
    -0.496562004089, 0.733016014099, 0.477212011814, -0.693643987179, 0.817578017712,
    0.357154011726, -0.908470988274, 0.371228992939, -0.495434999466, 0.850829005241,
    -0.144506007433, 0.933856010437, -0.867869019508, -0.721484005451, 0.84049898386,
    0.712010025978, 0.947140991688, -0.643828988075, 0.365539014339, -0.279448986053,
    0.397228986025, 0.0257009994239, 0.442745000124, 0.0326259993017, 0.241726994514,
    -0.646838009357, 0.227953001857, -0.156868994236, -0.3966139853, -0.782710015774,
    0.547042012215, 0.96251398325, -0.600062012672, 0.0840729996562, 0.0733639970422,
    -0.119843997061, -0.855558991432, 0.194073006511, 0.474844992161, -0.683000028133,
    -0.983412027359, -0.664865016937, 0.787952005863, 0.010758000426, -0.780273973942,
    -0.216953992844, -0.213891997933, -0.541665017605, -0.853213012218, -0.479514986277,
    -0.255351006985, -0.259918987751, -0.215997993946, -0.25065100193, 0.32811999321,
    0.650943994522, 0.682985007763, 0.322297006845, 0.916831970215, 0.457608997822,
    -0.686872005463, 0.523796021938, -0.775642991066, -0.524155020714, 0.927084982395,
    0.363738000393, 0.454650998116, 0.770654022694, 0.286431998014, -0.0467780008912,
    0.626110970974, -0.129168003798, 0.0923089981079, -0.383031994104, -0.124877996743,
    -0.802776992321, 0.617361009121, 0.616801023483, -0.193671002984, 0.954362988472,
    -0.887337982655, 0.885245978832, 0.546657979488, -0.498732000589, -0.809893012047,
    -0.757227003574, 0.361891001463, 0.508687019348, -0.116428002715, 0.545361995697,
    0.759167015553, 0.763499975204, 0.392697006464, -0.463658004999, 0.640906989574,
    0.00580500019714, -0.955587029457, 0.780699014664, 0.928444981575, 0.525066971779,
    -0.539120018482, -0.961925029755, -0.392244011164, 0.161203995347, -0.242228001356,
    -0.918917000294, 0.224405005574, -0.229423999786, 0.909914016724, -0.135256007314,
    0.206529006362, -0.778599977493, -0.0272630006075, 0.573095977306, -0.833109021187,
    -0.11641100049, 0.720070004463, 0.400754004717, -0.00689999992028, 0.579689979553,
    0.791122019291, -0.261225998402, -0.132616996765, -0.100652001798, -0.278663009405,
    -0.0817489996552, 0.55580997467, -0.790706992149, 0.24907399714, -0.265583992004,
    -0.332260996103, 0.500940024853, -0.233083993196, 0.79234701395, -0.674897015095,
    -0.034469999373, -0.452571004629, 0.865096986294, -0.192258998752, 0.715978980064,
    0.673097014427, 0.0106100002304, 0.481786996126, 0.201416000724, -0.110912002623,
    0.421380013227, 0.122218996286, -0.392324000597, -0.773620009422, 0.112406000495,
    0.634944021702, -0.895732998848, 0.600996017456, -0.423038989305, 0.604225993156,
    0.669169008732, 0.844263970852, 0.385176986456, 0.86817997694, 0.98620402813,
    0.338048994541, 0.843137025833, 0.50382399559, 0.356178998947, -0.0207509994507,
    -0.774716973305, -0.305014014244, 0.783281028271, 0.14514900744, -0.773391008377,
    0.485096991062, -0.331928998232, 0.404435992241, 0.866097986698, -0.358857989311,
    -0.563435018063, -0.870873987675, 0.410055011511, 0.395624011755, -0.392547011375,
    -0.474409013987, 0.706722021103, -0.0404729992151, -0.681576013565, -0.204821005464,
    0.485058009624, -0.971239984035, -0.206650003791, -0.387519001961, -0.89948797226,
    0.0303569994867, 0.244427993894, 0.415463000536, 0.0532939992845, -0.155333995819,
    0.328157991171, 0.374395012856, 0.0991830006242, -0.486932992935, 0.572492003441,
    -0.485525012016, 0.483934998512, -0.663926005363, -0.535557985306, 0.648835003376,
    0.0558190010488, 0.218388006091, 0.161862999201, -0.817974984646, -0.281639993191,
    0.140294000506, -0.251314014196, -0.626960992813, -0.0609150007367, -0.114887997508,
    0.927922010422, 0.920292973518, 0.387751013041, -0.912810027599, 0.950204014778,
    0.585726976395, 0.0626759976149, -0.579019010067, -0.415919005871, 0.247060999274,
    -0.569508016109, 0.580492973328, -0.00690799998119, 0.175663992763, -0.340445011854,
    -0.0420989990234, -0.769584000111, -0.0692740008235, 0.215324997902, -0.853244006634,
    -0.838041007519, 0.809980988503, 0.319032996893, 0.964784026146, 0.349070012569,
    0.830331027508, -0.739802002907, -0.695402026176, -0.23067599535, -0.228488996625,
    -0.0645819976926, -0.0642580017447, 0.663621008396, 0.857397973537, -0.947665989399,
    -0.960766971111, 0.5284730196, 0.817074000835, -0.472171992064, -0.8043410182,
    -0.0278849992901, -0.247139006853, 0.85051202774, 0.577008008957, 0.306573987007,
    0.451532006264, -0.84249997139, -0.892139017582, 0.483613997698, -0.581296026707,
    -0.931030988693, 0.507709026337, 0.126278996468, 0.562844991684, -0.638350009918,
    0.792176008224, -0.786127984524, 0.0657930001616, 0.0564580000937, -0.182858005166,
    -0.749339997768, 0.252077013254, 0.352898001671, 0.186030000448, 0.466933995485,
    -0.79059702158, 0.796787977219, 0.436091005802, 0.890829980373, 0.466033995152,
    0.705829977989, -0.467765986919, -0.595811009407, -0.397195994854, -0.849485993385,
    0.541548013687, 0.703132987022, 0.878014028072, -0.783003985882, -0.902799010277,
    -0.238875001669, -0.784695029259, 0.853748977184, 0.216802999377, 0.250429004431,
    -0.329243987799, -0.647345006466, 0.43973299861, -0.444184988737, 0.920058012009,
    -0.065424002707, -0.598731994629, 0.450237005949, 0.508762001991, -0.428122997284,
    -0.862047016621, -0.0856420025229, -0.825124025345, 0.50442302227, 0.430137008429,
    0.362204998732, 0.488447993994, -0.397906005383, 0.145673006773, -0.75032800436,
    0.73883497715, -0.874804019928, -0.932770013809, -0.454465985298, 0.474770992994,
    -0.723435997963, -0.911023020744, 0.566757023335, 0.332841992378, 0.834052979946,
    -0.240180999041, -0.684325993061, -0.728932023048, 0.345874994993, 0.266330987215,
    -0.4205250144, 0.821784973145, 0.281237989664, 0.579483985901, -0.870828986168,
    0.685931026936, 0.169338002801, -0.746029019356, -0.85235697031, -0.0115940002725,
    -0.451200008392, 0.915750026703, -0.0273320004344, -0.152633994818, 0.96387398243,
    -0.70207297802, 0.412086993456, -0.807394981384, 0.632058978081, -0.756034016609,
    0.457114994526, -0.678901016712, 0.211875006557, -0.920462012291, 0.525736987591,
    0.286430001259, 0.389450013638, -0.419557988644, 0.177065998316, -0.936475992203,
    0.0529859997332, 0.17698200047, 0.298745989799, -0.73995000124, 0.70370298624,
    0.726926028728, -0.861890017986, 0.717215001583, -0.899258971214, -0.556580007076,
    0.69909799099, 0.287068992853, 0.938157021999, -0.777517974377, -0.6680560112,
    -0.311062991619, -0.240915998816, 0.587373971939, 0.818193972111, -0.10667899996,
    -0.585089027882, 0.862704992294, 0.138329997659, -0.608277022839, 0.259418010712,
    -0.452078998089, 0.716067016125, 0.472270011902, 0.453597992659, -0.327021002769,
    0.188062995672, 0.307262003422, -0.00208500004373, 0.890700995922, -0.542182981968,
    0.90247797966, 0.608220994473, -0.361656993628, -0.400384008884, 0.231106996536,
    -0.0619449988008, 0.728648006916, -0.357187986374, 0.887198984623, -0.28876799345,
    -0.879347026348, -0.76706302166, -0.534757018089, 0.984449982643, 0.48469299078,
    0.106899999082, -0.697501003742, -0.269564002752, -0.79991799593, -0.372011005878,
    -0.811110019684, 0.249290004373, -0.242210000753, -0.371217012405, -0.741173028946,
    -0.818971991539, 0.854489982128, 0.598385989666, -0.567286014557, -0.851817011833,
    0.841499984264, -0.762871026993, 0.138466000557, -0.470479011536, -0.93360799551,
    0.76762598753, 0.971053004265, 0.0578299984336, 0.615401029587, -0.895838022232,
    -0.961593985558, -0.952329993248, 0.247762992978, -0.347243994474, 0.261171013117,
    0.0295690000057, 0.0993359982967, -0.498239994049, 0.650304019451, -0.00318300002255,
    -0.224086001515, -0.822551012039, 0.550083994865, -0.31110098958, -0.0982619971037,
    0.689499974251, 0.74392002821, -0.96017998457, 0.385711997747, -0.246020004153,
    -0.829455018044, 0.184842005372, 0.486759006977, -0.0229729991406, 0.135477006435,
    0.352414995432, 0.200444996357, 0.37894898653, -0.53509002924, 0.940702021122,
    0.593697011471, 0.37039399147, 0.0647080019116, -0.247896000743, 0.777836978436,
    -0.458617985249, 0.514841973782, -0.530234992504, -0.628462970257, 0.30958199501,
    -0.460213989019, -0.983359992504, -0.336533010006, -0.464080989361, 0.558609008789,
    -0.337814986706, 0.261164993048, -0.554709970951, 0.725816011429, 0.799120008945,
    0.304232001305, 0.609165012836, 0.702131986618, -0.108517996967, -0.611836016178,
    0.307417005301, -0.992465019226, -0.579057991505, 0.253778010607, -0.774506986141,
    0.610374987125, 0.313230007887, -0.291741013527, -0.0268580000848, -0.168054997921,
    0.15637999773, 0.339314997196, 0.695621013641, 0.346839994192, -0.857464015484,
    0.636233985424, -0.48317399621, -0.449730992317, 0.691825985909, -0.0794209986925,
    0.223572000861, 0.370669990778, 0.785915970802, -0.994440972805, -0.684360027313,
    -0.0590940006077, 0.194683000445, -0.190516993403, -0.451467007399, -0.47193300724,
    0.979997992516, -0.216608002782, -0.895137012005, -0.292237997055
};

static int particleRandomIndex = 0;

float particleRand()
{
    if (particleRandomIndex >= 1024)
        particleRandomIndex = 0;
    float tmp = particleRandomTable[particleRandomIndex];
    particleRandomIndex++;
    return tmp;
}

void particle_recalc(NC_STACK_particle::__NC_STACK_particle *prtcl)
{
    float v15 = prtcl->ctxLifeTime;

    prtcl->accelDelta = (prtcl->accel.end - prtcl->accel.start) / v15;

    prtcl->magnifyDelta = (prtcl->magnify.end - prtcl->magnify.start) / v15;
}


int sub_41A8D0(NC_STACK_particle::__NC_STACK_particle *prtcl)
{
    NC_STACK_skeleton *sklt = Nucleus::CInit<NC_STACK_skeleton>( {
        {NC_STACK_rsrc::RSRC_ATT_NAME, std::string("particle_sklt")},
        {NC_STACK_rsrc::RSRC_ATT_TRYSHARED, (int32_t)2},
        {NC_STACK_skeleton::SKEL_ATT_POINTSCNT, (int32_t)5},
        {NC_STACK_skeleton::SKEL_ATT_POLYCNT, (int32_t)1},
        {NC_STACK_skeleton::SKEL_ATT_POLYPNTCNT, (int32_t)4} } );
        
    prtcl->particle_sklt = sklt;

    if ( !sklt )
        return 0;

    prtcl->particle_sklt_intern = prtcl->particle_sklt->GetSkelet();

    UAskeleton::Polygon *v3 = prtcl->particle_sklt_intern->polygons;

    v3[0].num_vertices = 4;
    v3[0].v[0] = 1;
    v3[0].v[1] = 2;
    v3[0].v[2] = 3;
    v3[0].v[3] = 4;

    return 1;
}

void sub_41AB50(NC_STACK_particle::__NC_STACK_particle *prtcl)
{
    int v3 = (prtcl->field_8 & 1) != 0;
    if (prtcl->ADEs_count)
    {
        for(int i = 0; i < prtcl->ADEs_count; i++)
        {
            if (prtcl->ADEs[i])
            {
                prtcl->ADEs[i]->setADE_depthFade(v3);
                prtcl->ADEs[i]->setADE_point(0);
                prtcl->ADEs[i]->setADE_poly(0);
            }
        }
        prtcl->field_14 = prtcl->field_84 / prtcl->ADEs_count;
    }
}

void sub_41ABBC(NC_STACK_particle::__NC_STACK_particle *prtcl, NC_STACK_ade **areas)
{
    for (int i = 0; i < prtcl->ADEs_count; i++)
    {
        if (prtcl->ADEs[i])
            delete_class_obj(prtcl->ADEs[i]);
    }

    NC_STACK_ade **arr = areas;

    int n = 0;

    while(*arr)
    {
        if (n >= 8)
            printf("prtcl->field_18[%d] !!\n", n);

        prtcl->ADEs[n] = *arr;

        n++;
        arr++;
    }

    prtcl->ADEs_count = n;
    prtcl->ADEs[n] = NULL;
    sub_41AB50(prtcl);
}

int sub_41A954(NC_STACK_particle::__NC_STACK_particle *prtcl)
{
    if (prtcl->tp1)
    {
        NC_STACK_particle::Context *tmp = prtcl->tp1;
        while( tmp < prtcl->tp1_end )
        {
            if (tmp->tp2)
                nc_FreeMem(tmp->tp2);
            tmp++;
        }
        nc_FreeMem(prtcl->tp1);
        prtcl->tp1 = NULL;
    }



    int v5 = prtcl->ctxGenEnd - prtcl->ctxGenStart;

    if ( v5 > prtcl->field_84 )
        v5 = prtcl->field_84;

    int v7 = (prtcl->field_88 * v5 / 1024) + 2;

    if ( !prtcl->tp1_cnt || v7 < 2 )
        return 0;

    prtcl->tp1 = (NC_STACK_particle::Context *)AllocVec(sizeof(NC_STACK_particle::Context) * prtcl->tp1_cnt, 65537);
    if ( !prtcl->tp1 )
        return 0;

    int v17 = 1;
    if ( prtcl->tp1_cnt )
    {
        for (int i = 0; i < prtcl->tp1_cnt; i++)
        {
            prtcl->tp1[i].tp2 = (NC_STACK_particle::Particle *)AllocVec(sizeof(NC_STACK_particle::Particle) * v7, 65537);
            if ( prtcl->tp1[i].tp2 )
            {
                prtcl->tp1[i].tp2_end = &prtcl->tp1[i].tp2[v7];
            }
            else
            {
                v17 = 0;
            }
        }
    }

    if ( !v17 )
        return 0;

    prtcl->tp1_end = &prtcl->tp1[prtcl->tp1_cnt];
    prtcl->tp1_next = prtcl->tp1;

    return 1;
}


size_t NC_STACK_particle::func0(IDVList &stak)
{
    if ( !NC_STACK_ade::func0(stak) )
        return 0;

    if (!sub_41A8D0(&stack__particle))
    {
        func1();
        return 0;
    }

    startSetter();

    stack__particle.tp1_cnt = 1;
    stack__particle.ctxLifeTime = 1000;
    stack__particle.field_88 = 10;
    stack__particle.field_84 = 3000;
    stack__particle.field_90 = 30.0;
    stack__particle.field_94 = 30.0;
    stack__particle.ctxGenStart = 0;
    stack__particle.ctxGenEnd = 1000;
    stack__particle.field_9c = 50.0;

    for( auto& it : stak )
    {
        IDVPair &val = it.second;

        if ( !val.Skip )
        {
            switch (val.ID)
            {
            case ADE_ATT_DPTHFADE:
                setADE_bkCheck( val.Get<int32_t>() );
                break;

            case ADE_ATT_POINT:
                setADE_point( val.Get<int32_t>() );
                break;

            case PRTCL_ATT_STSPEED:
                setPRTCL_startSpeed( val.Get<int32_t>() );
                break;

            case PRTCL_ATT_NUMCONT:
                setPRTCL_numContexts( val.Get<int32_t>() );
                break;

            case PRTCL_ATT_CONTLFTIME:
                setPRTCL_contextLifetime( val.Get<int32_t>() );
                break;

            case PRTCL_ATT_BIRTRATE:
                setPRTCL_birthRate( val.Get<int32_t>() );
                break;

            case PRTCL_ATT_LFTIME:
                setPRTCL_lifeTime( val.Get<int32_t>() );
                break;

            case PRTCL_ATT_PADE:
                setPRTCL_pADE( val.Get<NC_STACK_area *>() );
                break;

            case PRTCL_ATT_STSIZE:
                setPRTCL_startSize( val.Get<int32_t>() );
                break;

            case PRTCL_ATT_ENDSIZE:
                setPRTCL_endSize( val.Get<int32_t>() );
                break;

            case PRTCL_ATT_STGEN:
                setPRTCL_startGen( val.Get<int32_t>() );
                break;

            case PRTCL_ATT_ENDGEN:
                setPRTCL_endGen( val.Get<int32_t>() );
                break;

            case PRTCL_ATT_NOISE:
                setPRTCL_noise( val.Get<int32_t>() );
                break;

            case PRTCL_ATT_PPADE:
                setPRTCL_ppADE ( val.Get<NC_STACK_ade **>() );
                break;

            default:
                break;
            }
        }
    }

    if (!endSetter())
    {
        func1();
        return 0;
    }

    return 1;
}

size_t NC_STACK_particle::func1()
{
    __NC_STACK_particle *prtcl = &stack__particle;


    if ( prtcl->tp1 )
    {
        for(int i = 0; i < prtcl->tp1_cnt; i++)
        {
            if (prtcl->tp1[i].tp2)
                nc_FreeMem(prtcl->tp1[i].tp2);
        }
        nc_FreeMem(prtcl->tp1);
    }


    for(int i = 0; i < prtcl->ADEs_count; i++)
    {
        if (prtcl->ADEs[i])
            delete_class_obj(prtcl->ADEs[i]);
    }

    if ( prtcl->particle_sklt )
        delete_class_obj(prtcl->particle_sklt);

    return NC_STACK_ade::func1();
}

int NC_STACK_particle::particle_func5__sub0(IFFile *mfile)
{
    prtcl_att atts;

    mfile->readS16B(atts.version);

    TFEngine::Vec3dReadIFF(atts.accel.start, mfile, true);
    TFEngine::Vec3dReadIFF(atts.accel.end, mfile, true);

    TFEngine::Vec3dReadIFF(atts.magnify.start, mfile, true);
    TFEngine::Vec3dReadIFF(atts.magnify.end, mfile, true);

    mfile->readS32B(atts.collide);
    mfile->readS32B(atts.startSpeed);
    mfile->readS32B(atts.contextNumber);
    mfile->readS32B(atts.contextLifeTime);
    mfile->readS32B(atts.contextStartGen);
    mfile->readS32B(atts.contextStopGen);
    mfile->readS32B(atts.genRate);
    mfile->readS32B(atts.lifeTime);
    mfile->readS32B(atts.startSize);
    mfile->readS32B(atts.endSize);
    mfile->readS32B(atts.noise);

    if ( atts.version >= 1 )
    {
        startSetter();
        setPRTCL_startSpeed( atts.startSpeed );
        setPRTCL_numContexts( atts.contextNumber );
        setPRTCL_contextLifetime( atts.contextLifeTime );
        setPRTCL_birthRate( atts.genRate );
        setPRTCL_lifeTime( atts.lifeTime );
        setPRTCL_startSize( atts.startSize );
        setPRTCL_endSize( atts.endSize );
        setPRTCL_startGen( atts.contextStartGen );
        setPRTCL_endGen( atts.contextStopGen );
        setPRTCL_noise( atts.noise );
        endSetter();
    }

    SetAccel(&atts.accel);
    SetMagnify(&atts.magnify);

    return 1;
}

size_t NC_STACK_particle::func5(IFFile **file)
{
    IFFile *mfile = *file;

    int obj_ok = 0;
    __NC_STACK_particle *prtcl = NULL;

    int v6 = 0;
    NC_STACK_ade *v23[11];

    while ( 1 )
    {
        int iff_res = mfile->parse();

        if ( iff_res == -2 )
            break;

        if ( iff_res )
        {
            if ( obj_ok )
                func1();
            return 0;
        }

        IFFile::Context *chunk = mfile->getCurrentChunk();

        if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_ADE )
        {
            obj_ok = NC_STACK_ade::func5(file);

            if ( !obj_ok )
                break;

            prtcl = &stack__particle;
        }
        else if ( chunk->TAG == TAG_ATTS )
        {
            if ( !particle_func5__sub0(mfile) )
            {
                func1();
                return 0;
            }
            mfile->parse();
        }
        else if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_OBJT )
        {
            v23[v6] = (NC_STACK_area *)READ_OBJT(mfile);
            if (!v23[v6])
            {
                func1();
                return 0;
            }
            v6++;
        }
        else
        {
            mfile->skipChunk();
        }
    }

    if ( obj_ok )
    {
        if ( !sub_41A8D0(prtcl) )
        {
            func1();
            return 0;
        }

        if ( !sub_41A954(prtcl) )
        {
            func1();
            return 0;
        }

        v23[v6] = NULL;

        particle_recalc(prtcl);

        prtcl->field_8c = 1024 / prtcl->field_88;
        prtcl->field_98 = (prtcl->field_94 - prtcl->field_90) / prtcl->field_84;

        sub_41ABBC(prtcl, v23);
    }
    return obj_ok;
}

size_t NC_STACK_particle::func6(IFFile **file)
{
    IFFile *mfile = *file;
    __NC_STACK_particle *prtcl = &stack__particle;

    if ( mfile->pushChunk(TAG_PTCL, TAG_FORM, -1) != 0)
        return 0;

    if ( !NC_STACK_ade::func6(file) )
        return 0;

    mfile->pushChunk(0, TAG_ATTS, -1);

    prtcl_att a1;

    mfile->writeS16B(1);
    mfile->writeFloatB(prtcl->accel.start.x);
    mfile->writeFloatB(prtcl->accel.start.y);
    mfile->writeFloatB(prtcl->accel.start.z);
    mfile->writeFloatB(prtcl->accel.end.x);
    mfile->writeFloatB(prtcl->accel.end.y);
    mfile->writeFloatB(prtcl->accel.end.z);
    mfile->writeFloatB(prtcl->magnify.start.x);
    mfile->writeFloatB(prtcl->magnify.start.y);
    mfile->writeFloatB(prtcl->magnify.start.z);
    mfile->writeFloatB(prtcl->magnify.end.x);
    mfile->writeFloatB(prtcl->magnify.end.y);
    mfile->writeFloatB(prtcl->magnify.end.z);
    mfile->writeS32B(0);
    mfile->writeS32B(prtcl->field_9c);
    mfile->writeS32B(prtcl->tp1_cnt);
    mfile->writeS32B(prtcl->ctxLifeTime);
    mfile->writeS32B(prtcl->ctxGenStart);
    mfile->writeS32B(prtcl->ctxGenEnd);
    mfile->writeS32B(prtcl->field_88);
    mfile->writeS32B(prtcl->field_84);
    mfile->writeS32B(prtcl->field_90);
    mfile->writeS32B(prtcl->field_94);
    mfile->writeS32B(prtcl->field_a0 * 10.0);

    mfile->popChunk();

    for (int i = 0; i < prtcl->ADEs_count; i++)
    {
        if (prtcl->ADEs[i])
            if ( !sub_4117F8(prtcl->ADEs[i], mfile))
                return 0;
    }

    return mfile->popChunk() == IFFile::IFF_ERR_OK;
}

void particle_func65__sub0__sub0(NC_STACK_particle::__NC_STACK_particle *prtcl, NC_STACK_particle::Context *tp1, vec3d *pos1, vec3d *pos2, float a4)
{
    NC_STACK_particle::Particle *cur = tp1->field_C;

    tp1->field_C++;

    if (tp1->field_C >= tp1->tp2_end)
        tp1->field_C = tp1->tp2;

    if ( tp1->field_8 == tp1->field_C )
    {
        tp1->OldestAge -= prtcl->field_8c;

        tp1->field_8++;

        if ( tp1->field_8 == tp1->tp2_end )
            tp1->field_8 = tp1->tp2;
    }

    vec3d v12 = vec3d(particleRand(), particleRand(), particleRand()) + *pos1;
    float v16 = v12.length();

    cur->vec  = v12 * (prtcl->field_9c / v16);
    cur->pos = cur->vec * a4 + *pos2;
}

void particle_func65__sub0__sub1(NC_STACK_particle::__NC_STACK_particle *prtcl, NC_STACK_particle::Particle *tp2, area_arg_65 *arg, float a3, unsigned int a4)
{
    TFEngine::TForm3D *glob = arg->view;
    mat3x3 *pmat = &glob->globSclRot;
    UAskeleton::Vertex *v14 = prtcl->particle_sklt_intern->tformedVertex;

    int v27 = -1;

    vec3d v31 = tp2->pos - ((glob->globSclRot.AxisX() + glob->globSclRot.AxisY()) * a3 * 0.5);

    vec3d v37;//, v36, v37;

    for (int i = 0; i < 5; i++)
    {
        switch ( i )
        {
        case 0:
            v37 = tp2->pos;
            break;
        case 1:
            v37 = v31;
            break;
        case 2:
            v37 = v31 + pmat->AxisX() * a3;
            break;
        case 3:
            v37 = v31 + (pmat->AxisX() + pmat->AxisY()) * a3;
            break;
        case 4:
            v37 = v31 + pmat->AxisY() * a3;
            break;
        }

        v37 -= glob->globPos;

        int v20 = 0;

        vec3d v30 = pmat->Transform(v37);

        if (v30.z < arg->minZ)
            v20 = 16;
        else if (v30.z > arg->maxZ)
            v20 = 32;

        if ( v30.x > v30.z )
            v20 |= 2;

        if ( -v30.z > v30.x )
            v20 |= 1;

        if ( v30.y > v30.z )
            v20 |= 8;

        if ( -v30.z > v30.y )
            v20 |= 4;

        *v14 = v30;
        v14->flags = v20;

        v27 &= v20;
        v14++;
    }

    if ( !v27 )
    {
        UAskeleton::Data *v22 = arg->sklt;
        NC_STACK_skeleton *v23 = arg->OBJ_SKELETON;

        arg->sklt = prtcl->particle_sklt_intern;
        arg->OBJ_SKELETON = prtcl->particle_sklt;

        if ( prtcl->field_14 )
        {
            int id = a4 / prtcl->field_14;
            if ( id < 8 )
            {
                NC_STACK_ade *v25 = prtcl->ADEs[id];
                if ( v25 )
                    v25->ade_func65(arg);
            }
        }

        arg->OBJ_SKELETON = v23;
        arg->sklt = v22;
    }
}

void particle_func65__sub0(NC_STACK_particle::__NC_STACK_particle *prtcl, NC_STACK_particle::Context *ctx, area_arg_65 *arg)
{
    ctx->time += arg->frameTime;
    ctx->OldestAge += arg->frameTime;

    float v59 = (float)arg->frameTime * 0.001;

    int age = arg->timeStamp - ctx->timeStamp;

    ctx->age = age;

    if ( age >= prtcl->ctxLifeTime )
    {
        ctx->field_18 = 0;
        return;
    }

    if ( age >= prtcl->ctxGenEnd )
    {
        if ( ctx->field_8 == ctx->field_C )
            return;

        while ( prtcl->field_84 <= ctx->OldestAge )
        {
            ctx->OldestAge -= prtcl->field_8c;

            if ( ctx->field_8 != ctx->field_C )
            {
                ctx->field_8++;

                if ( ctx->field_8 >= ctx->tp2_end )
                    ctx->field_8 = ctx->tp2;
            }
        }
    }
    else if ( age >= prtcl->ctxGenStart && ctx->time >= 0 )
    {
        vec3d v = prtcl->magnify.start + prtcl->magnifyDelta * age;
        vec3d v44 = arg->owner->globSclRot.Transform(v);

        //xyz v45 = arg->owner->globPos + arg->owner->globSclRot.Transform(*v19);
        vec3d v45 = arg->owner->tform * ( arg->sklt->POO[prtcl->field_c] );

        float v61 = 0.0;
        float tmp = prtcl->field_8c * 0.001;
        while ( ctx->time >= 0 )
        {
            particle_func65__sub0__sub0(prtcl, ctx, &v44, &v45, v61);
            v61 += tmp;
            ctx->time -= prtcl->field_8c;
        }
    }


    if ( ctx->field_8 != ctx->field_C )
    {
        int v30 = ctx->OldestAge;

        vec3d tmp = prtcl->accel.start + prtcl->accelDelta * age;

        NC_STACK_particle::Particle *v31 = ctx->field_8;

        while (v31 != ctx->field_C)
        {
            float a3a = v30 * prtcl->field_98 + prtcl->field_90;

            v31->vec += tmp;

            vec3d rnd = vec3d( particleRand(), particleRand(), particleRand() );

            v31->pos += rnd * prtcl->field_a0 + v31->vec * v59;

            particle_func65__sub0__sub1(prtcl, v31, arg, a3a, v30);

            v30 -= prtcl->field_8c;

            v31++;
            if ( v31 >= ctx->tp2_end )
                v31 = ctx->tp2;
        }
    }
}

size_t NC_STACK_particle::ade_func65(area_arg_65 *arg)
{
    __NC_STACK_particle *prtcl = &stack__particle;

    NC_STACK_particle::Context *ctx = prtcl->tp1;

    int ctxs = 0;

    if (arg->ownerID != ctx->field_18)
    {
        while( 1 )
        {
            ctx++;
            ctxs++;
            if (ctx >= prtcl->tp1_end)
            {
                ctx = NULL;
                break;
            }
            else if (arg->ownerID == ctx->field_18)
            {
                break;
            }
        }
    }

    if (ctx)
        particle_func65__sub0(prtcl, ctx, arg);
    else
    {
        NC_STACK_particle::Context *cur = prtcl->tp1_next;

        if ( cur->field_18 && arg->timeStamp - cur->timeStamp < 1000 )
        {
            ctx = NULL;
        }
        else
        {
            cur->field_18 = arg->ownerID;
            prtcl->tp1_next++;

            if ( prtcl->tp1_next == prtcl->tp1_end )
                prtcl->tp1_next = prtcl->tp1;

            ctx = cur;
        }

        if ( ctx )
        {
            ctx->OldestAge = 0;
            ctx->time = 0;
            ctx->age = 0;
            ctx->field_8 = ctx->tp2;
            ctx->field_C = ctx->tp2;
            ctx->timeStamp = arg->timeStamp;
            particle_func65__sub0(prtcl, ctx, arg);
        }

    }

    return 1;
}

void NC_STACK_particle::SetAccel(StartEnd *arg)
{
    __NC_STACK_particle *prtcl = &stack__particle;

    prtcl->accel = *arg;

    particle_recalc(prtcl);
}

void NC_STACK_particle::SetMagnify(StartEnd *arg)
{
    __NC_STACK_particle *prtcl = &stack__particle;

    prtcl->magnify = *arg;

    particle_recalc(prtcl);
}

void NC_STACK_particle::particle_func130(StartEnd *out)
{
    __NC_STACK_particle *prtcl = &stack__particle;

    *out = prtcl->accel;
}

void NC_STACK_particle::particle_func131(StartEnd *out)
{
    __NC_STACK_particle *prtcl = &stack__particle;

    *out = prtcl->magnify;
}
size_t NC_STACK_particle::particle_func132(NC_STACK_area **ade)
{
    __NC_STACK_particle *prtcl = &stack__particle;

    if (prtcl->ADEs_count >= 8)
        return 0;

    prtcl->ADEs[ prtcl->ADEs_count ] = *ade;
    prtcl->ADEs_count++;
    sub_41AB50(prtcl);

    return 1;
}

NC_STACK_ade * NC_STACK_particle::particle_func133(int *id)
{
    __NC_STACK_particle *prtcl = &stack__particle;

    NC_STACK_ade *ade = NULL;

    if ( *id < prtcl->ADEs_count )
    {
        ade = prtcl->ADEs[*id];

        for (int i = *id; i < prtcl->ADEs_count - 1; i++)
            prtcl->ADEs[i] = prtcl->ADEs[i + 1];

        prtcl->ADEs_count--;
        prtcl->ADEs[prtcl->ADEs_count] = NULL;
        sub_41AB50(prtcl);
    }

    return ade;
}

size_t NC_STACK_particle::particle_func134(int *iid)
{
    __NC_STACK_particle *prtcl = &stack__particle;

    int id = *iid;

    if ( id > 0 && id < prtcl->ADEs_count )
    {
        NC_STACK_ade *ade = prtcl->ADEs[id];
        prtcl->ADEs[ id ] = prtcl->ADEs[ id - 1 ];
        prtcl->ADEs[ id - 1 ] = ade;
    }

    return 0;
}

size_t NC_STACK_particle::particle_func135(int *iid)
{
    __NC_STACK_particle *prtcl = &stack__particle;

    int id = *iid;

    if ( id >= 0 && id < (prtcl->ADEs_count - 1) )
    {
        NC_STACK_ade *ade = prtcl->ADEs[id];
        prtcl->ADEs[ id ] = prtcl->ADEs[ id + 1 ];
        prtcl->ADEs[ id + 1 ] = ade;
    }

    return 0;
}


void NC_STACK_particle::setADE_depthFade(int mode)
{
    if ( mode )
        stack__particle.field_8 |= PRTCL_FLAG_DPTHFADE;
    else
        stack__particle.field_8 &= ~PRTCL_FLAG_DPTHFADE;

    if (msetter)
        updateflags |= 8;
    else
        sub_41AB50(&stack__particle);

    NC_STACK_ade::setADE_depthFade(mode);
}

void NC_STACK_particle::setADE_point(int num)
{
    stack__particle.field_c = num;

    NC_STACK_ade::setADE_point(num);
}

void NC_STACK_particle::setPRTCL_startSpeed(int spd)
{
    stack__particle.field_9c = spd;
}

void NC_STACK_particle::setPRTCL_numContexts(int num)
{
    stack__particle.tp1_cnt = num;

    if (msetter)
        updateflags |= 1;
    else
        sub_41A954(&stack__particle);
}

void NC_STACK_particle::setPRTCL_contextLifetime(int lftime)
{
    stack__particle.ctxLifeTime = lftime;

    if (msetter)
        updateflags |= 1;
    else
        particle_recalc(&stack__particle);
}

void NC_STACK_particle::setPRTCL_birthRate(int rate)
{
    stack__particle.field_88 = rate;

    if (msetter)
        updateflags |= (1 | 4);
    else
    {
        sub_41A954(&stack__particle);
        refreshParticle();
    }
}

void NC_STACK_particle::setPRTCL_lifeTime(int lftime)
{
    stack__particle.field_84 = lftime;

    if (msetter)
        updateflags |= (1 | 4 | 8);
    else
    {
        sub_41A954(&stack__particle);
        refreshParticle();
        sub_41AB50(&stack__particle);
    }
}

void NC_STACK_particle::setPRTCL_pADE(NC_STACK_ade *ade)
{
    NC_STACK_ade *v7[2];
    v7[0] = ade;
    v7[1] = NULL;

    sub_41ABBC(&stack__particle, v7);
}

void NC_STACK_particle::setPRTCL_startSize(int sz)
{
    stack__particle.field_90 = sz;

    if (msetter)
        updateflags |= 4;
    else
        refreshParticle();
}

void NC_STACK_particle::setPRTCL_endSize(int sz)
{
    stack__particle.field_94 = sz;

    if (msetter)
        updateflags |= 4;
    else
        refreshParticle();
}

void NC_STACK_particle::setPRTCL_startGen(int gn)
{
    stack__particle.ctxGenStart = gn;

    if (msetter)
        updateflags |= 1;
    else
        sub_41A954(&stack__particle);
}

void NC_STACK_particle::setPRTCL_endGen(int gn)
{
    stack__particle.ctxGenEnd = gn;

    if (msetter)
        updateflags |= 1;
    else
        sub_41A954(&stack__particle);
}

void NC_STACK_particle::setPRTCL_noise(int nz)
{
    stack__particle.field_a0 = nz * 0.1;
}

void NC_STACK_particle::setPRTCL_ppADE(NC_STACK_ade **arr)
{
    sub_41ABBC(&stack__particle, arr);
}


void NC_STACK_particle::refreshParticle()
{
    __NC_STACK_particle *prtcl = &stack__particle;

    prtcl->field_8c = 1024 / prtcl->field_88;
    prtcl->field_98 = (prtcl->field_94 - prtcl->field_90) / prtcl->field_84;
}

void NC_STACK_particle::startSetter()
{
    msetter = true;
    updateflags = 0;
}

bool NC_STACK_particle::endSetter(bool recalc)
{
    msetter = false;
    if ( recalc )
    {
        if (updateflags & 1)
        {
            if (!sub_41A954(&stack__particle))
                return 0;
        }

        if (updateflags & 2)
            particle_recalc(&stack__particle);

        if (updateflags & 4)
            refreshParticle();

        if (updateflags & 8)
            sub_41AB50(&stack__particle);
    }

    updateflags = 0;
    return 1;
}

int NC_STACK_particle::getPRTCL_startSpeed()
{
    return stack__particle.field_9c;
}

int NC_STACK_particle::getPRTCL_numContexts()
{
    return stack__particle.tp1_cnt;
}

int NC_STACK_particle::getPRTCL_contextLifetime()
{
    return stack__particle.ctxLifeTime;
}

int NC_STACK_particle::getPRTCL_birthRate()
{
    return stack__particle.field_88;
}

int NC_STACK_particle::getPRTCL_lifeTime()
{
    return stack__particle.field_84;
}

NC_STACK_ade *NC_STACK_particle::getPRTCL_pADE()
{
    return stack__particle.ADEs[0];
}

int NC_STACK_particle::getPRTCL_startSize()
{
    return stack__particle.field_90;
}

int NC_STACK_particle::getPRTCL_endSize()
{
    return stack__particle.field_94;
}

int NC_STACK_particle::getPRTCL_startGen()
{
    return stack__particle.ctxGenStart;
}

int NC_STACK_particle::getPRTCL_endGen()
{
    return stack__particle.ctxGenEnd;
}

int NC_STACK_particle::getPRTCL_noise()
{
    return stack__particle.field_a0 * 10.0;
}

NC_STACK_ade **NC_STACK_particle::getPRTCL_ppADE()
{
    return stack__particle.ADEs;
}


