//
//  main.cpp
//  day01
//
//  Created by Kjell-Olov Högdal on 2023-12-01.
//

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <utility>
#include <optional>
#include <map>
#include <algorithm>
#include <string_view>

auto const NL = "\n";
auto const T = "\t";
auto const NT = "\n\t";

extern char const* example;
extern char const* puzzle;

using Answer = int;
using Digit = char;
using Entry = std::vector<Digit>;
using Model = std::vector<Entry>;

namespace part1 {
    Answer solve(auto const& input) {
        std::istringstream in{input};
        std::string entry{};
        Answer answer{};
        while (std::getline(in,entry) and (entry.size()>0)) {
            using Digit = char;
            std::vector<Digit> digits{}; // TODO: We cant buffer and delete as written numbers may overlap (e.g., "eightwo")
            for (auto const& ch : entry) {
                if (std::isdigit(ch)) {
                    digits.push_back(ch);
                }
            }
            if (digits.size()>0) {
                auto [first,last] = std::pair<Digit,Digit>({digits[0],digits.back()});
                std::cout << "\n" << std::quoted(entry) << " ==> " << "[" << first << "," << last << "]";
                Answer calibration_value = (first - '0')*10 + (last-'0');
                std::cout << " calibration value " << calibration_value;
                answer += calibration_value;
            }
            else {
                break;
            }
        }
        return answer;
    }
}

namespace part2 {
    auto parse(auto& in) {
      Model result{};
      std::cout << NL << "Parse advent of code 2023 day02";
      auto to_digit = [](std::string_view const& sv) ->std::optional<Digit> {
        std::optional<Digit> result{};
        std::map<std::string, Digit> digit_map{
            {"one",'1'}
            ,{"two",'2'}
            ,{"three",'3'}
            ,{"four",'4'}
            ,{"five",'5'}
            ,{"six",'6'}
            ,{"seven",'7'}
            ,{"eight",'8'}
            ,{"nine",'9'}
        };
        auto iter = std::find_if(digit_map.begin(), digit_map.end(), [&sv](auto const& entry) {
          return sv.starts_with(entry.first);
          });
        if (iter != digit_map.end()) {
          result = iter->second;
        }
        return result;
        };

      std::string entry{};
      while (std::getline(in, entry) and (entry.size() > 0)) {
        result.push_back({});
        std::cout << "\n" << std::quoted(entry);
        for (int i = 0; i < entry.size(); ++i) {
          auto ch = entry[i];
          if (std::isdigit(ch)) {
            std::cout << NT << "ch '" << ch << "' is digit " << ch;
            result.back().push_back(ch);
          }
          else {
            std::string_view at_i{ entry.begin() + i,entry.end() };
            if (auto digit = to_digit(at_i)) {
              std::cout << NT << std::quoted(at_i) << " is digit " << *digit;
              result.back().push_back(*digit);
            }
          }
        }
      }
      std::cout << NL << "result.size() = " << result.size();
      return result;
    }

    Answer solve(auto const& input) {
      Answer answer{ -1 };
      std::istringstream in{ input };
      auto model = part2::parse(in);

      for (auto const& digits : model) {
        if (digits.size() > 0) {
          auto [first, last] = std::pair<Digit, Digit>({ digits[0],digits.back() });
          std::cout << "\n\t== " << "[" << first << "," << last << "]";
          Answer calibration_value = (first - '0') * 10 + (last - '0');
          std::cout << " calibration value " << calibration_value;
          answer += calibration_value;
        }
        else {
          std::cerr << "\nNo digits in entry ";
          break;
        }
      }
      return answer;
    }
}

int main(int argc, const char * argv[]) {
    // auto part1_answer = part1::solve();
    // std::cout << "\nPart 1 = " << part1_answer;
    auto part2_answer = part2::solve(example);
    std::cout << "\nPart 2 = " << part2_answer;
    std::cout << "\nBYE :)\n";
    return 0;
}

char const* example = R"(two1nine
eightwothree
abcone2threexyz
xtwone3four
4nineeightseven2
zoneight234
7pqrstsixteen)";
/*
char const* puzzle = R"(threehqv2
sxoneightoneckk9ldctxxnffqnzmjqvj
1hggcqcstgpmg26lzxtltcgg
nrhoneightkmrjkd7fivesixklvvfnmhsldrc
zhlzhrkljonephkgdzsnlglmxvprlh6n
594chhplnzsxktjmkfpninefiveczfnvsctbxcfzfzjh
seven2tjf
five712
tzheightzlzmsqlnxfqzrlbhbdpbnbdjns6
ztwo1eight95
one25eightzptqnvvlnf
46brthree51bhvhtcnpcffoursix
five7bhsfdktxq33qtrmvqxfgone3
4f6six1skrmltxeight
onemhfbhrx99
five3eightjkdpxqtmbtwo
onebzp4seven4ft
5sxgjszzninembrtjptlfn24llbxrnm
sixninefivejpqgkcx3sixnine15
fourcmsndtdkrone75
68hnrkxbmvjzjzfk
2oneseventhreesixrbkkbhlx
five6npfmggbdkljqsixjnxgk1cqdmcneight
6eightfiveksmrbjgc
eight8zqjsixfivefrrbg4
2sixfhbnqbjv3krgqgfj
one56vhgnlxfjhrc6
8twothreelgbmx31
kfkjfckbhnv8threedpxhhfivepkcnfjmcpt
onenxqtvzvzph8nmtjgsjkone4btzxpkjfbthhsmqcqz
mdbsvbf8zbllzcsixclrjnfzf
knptlbzlffourfivetjzjpleightsix31
one6pbdfdtsqhjhqfourqqvkvksm
nxd3dkj1threeeightqxlfndjtglnxlqmb9
5three37tfnkz
57eight9fivefiveeight
2zd155
3lljdglhbfiveeight92
44lseven6
9xlv5gqgvpjvrhbbcrnmq2pt5
fiveeightslvzrvvgfsrvnsnhk1
twotsffgflt1two9four24three
2fourstszszgmxfive6fourzx
nine8bgh921seven
threevbvcrmqfivesix6fivenrxd22
zmdhdgfrr4przvs1
jpxt9
five6fivethree2three
qmz45cbtvx535
7eighttfbrsmvtpnsjcjpsjxcnine67two
nllgnjsdlkkng6ninesixjpjlmjg
two6615sixg2
7onetwoninemhjlqmlkkt
fourfffqcsmxc5nsixpncjnvpm5
rbbmltgpsixfive8nine4sevenfour
vfzmncfonexxkzlcstqhxvtwoplsglsix1kpkssfz
twojsrsixone39eight
three5nine
fcgx5pvnsxnhfd
9krnchzvnv8one
ninetwo1
eightrtvgkfjn1tfgxqeightseventwo
255five9
bxkg53
5vdr8
threetwomxbgqhzdq3
6dlg51p3
q53
8seven6594five
sixeightnpdsfdjljtwofiveqm7two2
qxsrbthree24four2
jbzeighteightpksq1twokcsvsr8
7kmmkdcmbl
twonvfhlseven7qksvksixqfz
8hbffjsbckqcscsevenbztnmvvg
one5kntjln21
2sevenlt7threesix49jkdtqqr
6drqlvfour1nine
6zmk8n
one279seven
65eightssvjkchfhrpdhlmrbrsevenkrpt7
dj87
sixsevendxvjjseight4two3tpvbcbf
4eightzz8three5
95gsfb5sevennhgmdc
plpn2threesix
seven22
8eightqqcgzd6ldh
4sixvsmhtlfcmbnmzgnsixfour
kmncbxcgmxmt1nine
4kb6one
threefivefivefourfive9npqtvvr3
cjkpsc5ninepkcqpvnkzcr
glzvgvpjvhsixbgdvzfive97mdbxtlvz7srhtpsfk
8three9dnzfgjzmzhh5seven
sevenfkqpshm6mx
9onedbvnnmfl15
sstcrnkkbzfsfcnkone8
xtqzchjspfdtsoneninethree4sixffsix
584ptgjtlqktk
kknpxzc26
pghjgrxh4135ttxr
2pjgp8one49
five3qjqsdstkq5ninevfzjgkpxphone82
fllgbdnbsztbfnjmhone7five
cgpqqcbfksnvppdqqsgh7twotzqglbvptmfive
2three6twozchrbnjsxdqtmfppzmr34
89kfjsix
3mpk7
3threekcgjfkk425
39zxtkntmxt
13ninevvrsxn436fivethree
nine5dphhllslr8nktvcd
7fourfourfivevbnlgzgxnpt
2hjvmsxfmfxtlljnqzr
6onepjqmjlsvfourfour2three
5kzsfrfmflp7twoqb4two3
three186threeone2
8four5hnineppprpkjhf
1qvtx1843
qblf36xsrdsvqhrmlrbthree
326sevenfivenseven1kctgmnqtwonefq
41sixfour
three4sevenone
4sevenptjmqgffivesix71pgpfcmc
14mndmdlsjhtprtxtskmdfive
6lrjglxqksevenkvsp5six37three
ljzfourjcclgcdgqcdnbhk9mrtnfrtwo
389six37threervldzxvlgr
2rtjhvsvrmtwo1hgzpeightdjkhxhmseven7
seven38fivepknvlmrjgbms7
27eightlccjptfm5rdthree4
cqpnjv58
2mbvkpzvfccrns8one52
three4seven7two
dfptdxdj8flxhqvfg8zljld
dm71two1hnggncbqkxsfkmq7
8blsrrqrjlckv7xszllqddzn5oneightfg
5fivej
fvx97jjfc63qbkpnqqqzbfmj
tgnzdkgfoureight9
37kc
1shthsfsdfmkmgxh2
6fdldj
sevenv79spttnzrrbvpmcf
ghpxgf3xk1zddlhfdnzthxmqqmzdpf
9f7ninethreeone
288vxrjxkjr78lsgbcssfdtsix
6vkckltsqvhsgvhtonezmrdfvn4
12bhseven6
three8bxfndbnine
4three14one
qs9twotkqgxthree
kbtwonesix2eight194four
nxjseven7
threejpzxfiveninet9fivetkxgzxd3
8fjjbghh9
nine8fourfourthreempcldmm
six24qkjpkqlrrqfiveninejnjbdld
zhsztjsgthreeeightone831
9threegpgtdm
8mnl31two7oneseven
gxdltwoonelzvbctwo91fourthree
lmgfive8
41six13kcvdsix
qsix9qlzjxfive8ninengxncsdpck
twoqmzfsrdq7
vqsgtzqmvslp1vnnjzvkjggn9fiveninesix
blrrckhs7tfpnssnpk76vkdsthree
znoneight925cnqsm79lplfpmzzh
twozqv3fourtwo
mscksglc3ql
fxf3four8dfmncxqseven
pdrfptqj3ndpmfiveonecz436
mtzzzjz23zhcbg1
74tflvhzjhvfivefour5bjhhkfvrfm7three
3246three
zzzzxptgsevenfoureight3sevenpq89
35sevenzgpptx8trrhb7seven8
onegfshppnb3vpvsbcvsg2qndxmpm
6ntfjnvdrmrzhdhgljq
eightlnmdkqb1
sevenseven2three
pjxvzpdtfcnine9five
sevenseven7
9rhdtwoninemclzzqhztxhljmjssevenfive3
6eightsthxqvfzgrlffsix7qxxdktsbseven
nine938zrsrhn3
two1fourthreetwotkvktvsspzfbfhrch27
29one7672
8zcdxmbfrhj3bqncflspd
bnplfntoneninesix6one
8one2six3one3nseven
seven31
kcfsnqz52nnkxrblnsndpqqjonesix97
825seventbpqbx
8348
4dreight53rmcjrrtpcmcslbkbvphvnp3
eightb3pvjcczsdkghdkvjkxfpjxkgqjqgkdd5
sp3onejffrxblxck5one4vbvseven
lpsrh6five3twovvthree6
skhpb21zptmbvccrfcs9eight
pqfphhcgxz8eightwohv
ftbmkdgtkvvbjgpclnhrlxphgrph52five
two7xkccqlcdcftzmnine6sgtt
bbdcstgccbfivefivesix7
xgzvtqbjngzk7sevenvmllqm4rldvmdvmtwotwo
one5four3eight2five6
cnfcdcv6onetwoseven24one9
cpgthkmbqd171ninedonetwosxvt
onemrrzcszddxcqvfive72drksvfcbpmseven
twothreetwo1
nmsrdffmhrjjhksix54five9eight
ftmjgrhlqxnine8two2ninefive8bxlhqv
twokjp3nine
6mzrgjbf51onefourthree
threenine89zlmh9fourff
threefourfivej6ptg
864dvsfvcvhtrqzgspsbvgvvmpgjsppsvsbxrr5
2sixzcnqbsixvcqnvnd
92lthree2
2onegmhhnljgrf9
847one3
sixeightseven2gbvgmmmlgbmkxdnkkfqj
bhxdczcbpfmhjprbrqqcfcdpeight1fbbtgnc9
two1npqxpbrhlrfourccsfrjggltslqlhjqthreezbc8
dgzm48
svhdzdtrctppcb6flhpvvnpsk3
s8fourrkrlhjsnsrddhrsxzseven
m4
8rctpll4ninefds64jxl2
two9seven36
vzrmfgtmjsixfourone4fourfrnr
7sixpzbdmvfddfhqspkninepxrcltthree4mjbh
6233q32
gzpmvphfl5six27one4eight
lclqftpzgqqbls83nine9seven
fzztdjhsixnine91
9five3
51seven2k
nine6vcssmvvld4fourff6
bjrplzdf9seveneight1
82rzzlqbr8ninesmfour39
kzgcnine24
815nine6mzjpkvfhdp5
84mclnine5
phhclzqllq581vhrsk7sixeight
8vccclqxdskgvgqoneseven
szm748dqvcxlbrqbzbdnqtqsnrcpcsqtgk6
686135
fzlvnbnfctseven6eighteightxkcstfv7qjsxpfxtfzpbcsm
vmsxmcmffourfour1
lbthreednlvv16fourxzhsix
eightfjtpfffour4eight9nine
8ksonefivefour
3sevenbcsfour52
7fourzflkvskjrcxxbdfz4nkjjbgnfour5three
nine9xnbjnnine2
onejzxkqt83twoxgltxrmztlfh5
jtkqjfourthreefour2seven897
8eightvvjdmfourgfmthreedgq5
89two79twokxfdsk
onesevenfour7gflgnsmpkjmlvntxhhqtqtwo2
three71eight3qq62xmkzfq
3mvn86
9mgsz
36ninehv62
vdtf7fxjgjz
sevenxlzdftxv87fourtwo
fivejgzhpfkths6four
52fourfour7sxzptgtnhsfour82
ddeightwohxxcninelzptfbxczrkhmxnvt2249mn
xlnbvzsxjfive94ninemsb3qvkvqk8
kfbvvpdxrrx1threecqhqxlqnts
two7nine
2jf
six71lzjldpjqssxhljslsrthreefour
5cqcdtpsqbplkdtbtbsevenseven1two
2threeseven
1zthreenblctql9nine5
eightmfive714foursix3
1hfbdtwo
3czdksdsjkvcstltsbjfour2
7four6
63pggqnfg9flspklblvxlg
oneeightnxdfsjgkbvxonetwonine2
674tdqqklvthreepnlschj39
jk5ninerpfdlfmpfmczg3
qggjm3pkbskbshjbztvzxllq8six8
plftsevenxxmfourone15
seven6eightsix86onenine
2spngskr19nine1sevensevenksz
bjqbjmcrbjdfnnnhmhsmdmdksevenninermlfive3
lfxj63three
qsnvrckf4mjnoneightlgt
tfqqh3ninegbcdnine8hv6xl
41fczmhgsixtwo
4qmnxhnhppjhptstwothreecmxxhhfflxpxb
ninejbjonexkfcm13
tgvkft8htone7
99dqllhxc
five7mxnhb216zkjmmnjmqrplgpg
76gfcvlzclzcnthdkkk532frtlllmsxbv
6bzrvlgdbkqktzjsdsjgqmfkzdckeightcsjsrrpcl
qnlk4sixlfqlm8
1sfxdrmonectrgngdnmbfglpr
2974rbfourfour2
98nine8three1qjftl4
1ninelqqnpfivefour8xgzbt
eighttwo1one
1threepxdnmpmtt
qqfcvv5zsgcpdgh
3ninegcnrtwotljnbkkftfourthreefour
vc1nhcmvvqsix6two
kmpfbntsix6jdgt8bdvbfptsbkjbbvnsbrjzzhqr
three3fourlxnn4
8ninetwofour2zxgrbhjxpgbjgtpn76
six8six
qsrxjhjrk5
eight7sxmmmghzpmclkcbqkxvkdfnrnmqhhkkt
nzrgxcfl9
4sixtwo2five
sevenrpbckzzxhcfour48two
pbmlptjqgcxcbpslkggvqgzdpqlvztwofour9ksmzpmshcctwo
811sqcbjrnpxbgv
one6bbml7dbmpfivepgrn
hmfive48onevbfxrlnck4gnb1
2threegklleightworcg
zoneightphzl6lksqsxklcqthreeeighttwonsnnrtqx
5crfbgsevensixdfour92
5eight122
six3gcjlxrr
1vbzx2onenvplbkmcfzmmbbqrone
oneninebctxhv78
6xpcdqcnxq
sixlhkqjgmlvcbjph6threetrlpnkg
nsxvf31ffqlx5four7tpnzqvllkn
onesix4dgskfddrfivefourtrzzxfjg
bprxxzdd1
three45shjhqtthreekmvx
sevenjpmjthreefive8eight9txtltqjm
zbqsnhfnkcccseven5vfiveonexvlfbkzbd
6kprj6eight4jhgdgcbcfg3lcfbgfsdkg
fourtwo217ljzlsevenznbmgd
jxpszskeight5rfourone2lxphhpone
five4zj5pz5qfoursix
vncrqcsix3fourxsnf7
636qbxtkzroneighttpv
1oneightgqx
two2fouronehcq
vbsmfthreethree7mhfsgnf
nklpxknvbbrjbmxtxplkldvspspvlq5tpbsxklqmndmxxn7
3four2684
3sixonethreegbjzfour2mmrk4
jzzrkn5five
ninesixsk1kphntgbmzlctjtzdmgzk
fourvjhkzqlqfour6c1
38xxrrgdrgxrninefive91
15seven8
nine4one5onenineone7
5mfknkone4cphtbrtj1eightwon
vgeightlvvxlkqzzgtg368nlmmhdmmqf
8threeeight29jhhqjpnbbp66mlb
mzzm4
sevenfive44xvxlrxzfk
82seven1twozdbpks143
eightckfbqfgz9gzkxqmzjkhsix7eight
tzl184two6four
fkdqzcj3three64seven1jrrdvcx
two6jq9ninenine
2three2three
7rsix73two
eightrmkhpvkdhd3four2twoseven
knhjndmbtwofive5
62three72xk
jxjzgqszseven2
8l4eightndnsssf
gpbqt882hsszgmsxc6two
ppeight9four
two9two96two3
fiveglpg41fourtwo7two
jtwonedjjninesvzc96
ztzgkneightdqpvgbnhlzhvchj65
757seven2tjzvtmbbgbxxrmtqvtp
25fivezdpdvgtlql8
sg2bktwo6fourfourtwo
1six6sevenjkpgbdqnrsvlbmq
honeight65five3
nine52kjhsrzxnp
five33
qtmcbxxjceightnmjxblsixonelhbspf5
twoltcsmtvmone1
1739one9
13ghnine8z
sixfive13twotkslfhxjfdqdhcj
vzjngqthreethreenmlshs89
sixjgnt1
onethree776gzsmt
4sixxxhnq
krnlpckgjv722threesxrcjkcznb6foursix
fkvmfourvkghfivesix3nine
sixmsrlzbdmnclztrhhtlbj9ndgccjq7d1
7hphone1
dlhdjjcpl443rsklgp
lljhthreesix53bzm8
4sixnine2four7threeone
237nine1three
lbplmntwobxpvcbslp6hpzpbmkvbqfour
onecgtqtbn787
one8threetwortvpqqspbgfour6nine
672twoone334
four4sixxghvzxm2
sevengcng8
eight9six5sixoneptdrglllzvsrtlc
sixxvdhxdlxdtdqjz24q
1gmkthree3kqgfdkmtppshvcjzqmstwo2
eightfourvkrzsqxq1sevennhnine5lmptlxl
sixckmdbd1mrncvrcntlvjcv
xptwone3xtgfourtwo2k5
4nineninetdl1fiverpfhsbvnbpzxbvhmqpqj
877tjrtdr
n286rkmsvkzcthreeqdxdccmcd
1979
254zhtrlvpfs
5njg
1nineqftrhzkxvgtwo
4five675zfjrhkdkbd
tzxrrzc8dljzcnmxmfourthree
fivezcgfdvrrf2sixseven2
74nine
387fourseven2twothree
9six5
threefnrqfkqtkxqlpt67
66rzpmsvztwonernf
9nlfsdtn8
89zbphtctxkfqpj6
eightksevenchjgvctdkfbhrxssfzcgssix21oneightrt
drmfmxpnptqt5lmdrnhzkfcknltj4
fourlvtkxjlheight3nine
kmbftcbrxpxqk9
mlgeightwo1rslfrhvfdgsprbmnqpp
ktwone98three
4hvxgtjdgldjfdnfiveeight
gmcsixthreenffhcbnc35six
xt8threefhgjrmpgh2
pj2sv77sqgjfhzx5
qccfftqjmv35
pnrsqpsgdlgzszldvtjrfive1two4seven74
4nbprtcnvpvcvdg5seven85bnvnrznnncbrsh
qmfmkqsix8fourhnfhqn69
threefpgk3seven
1rngbxkxqthkvonethree6seven
7three56qlqcrsljeight1five
18717fzdccbglhjmvkznqvfive
lprmxkdnrchs27sevenone6
four55sixjvhbkbpdjptnhcxpmlnfour
eightfvdccbgxrg4bplqrvvpjq59
one3rxm7eightxtg41
fourhxplrbq7mnvnlsdcfpkdknqdtdjrpfvkp
7mpmrgkqfzeight
three9cp
bk3eightt
4glcfxkptsixjfqdnscjv2xthree8
bdgxxxssthree4vfqzlvdk5
eighttwoonefsthree35
sflvqvvzcs271
cpflsnhjrmzqmfour3rhkdtqvskt
ncdgjdpdtpgpztj3
ninevlgzpcjsixthree49
fqfh7one2eight
vvzeight16three8kqvqzj
nineb4fdmtlhmjbsxzrkblxm2tpqkn
zdtghhxfbkkdnsix7sfj
eightfldsxvsvmdl6onenxfsz
threeprbbbcxhqone9jtqjone27six
pstg7
2threethreefive4nine
4fourthree
hgj1npvqhmkm
43twonine
58t2tvtstgs8tgmfbndr
9xdvnrzgnz6dkvnk5
one31nineeight
1eightgzzmrqjmxtdxmxcxn5txqvhdr
four1seven
sixz12cvqlmqqzvnnhfiveoneightm
three5fznzbvsqnxtqnsmjt7hrhjgvqxnr
gmbhone5fpxjlvksevenjgh
fjfskl4
tkjqkpjddlmvmmxtxf5mdzgjone42
onesflmvlhfvxlrbk68
gfive9zhtbkvn
xxhsvbjxhcnqsnvnineone489nft
ninetwo8fiveeight1ninennls
xdkbsfrfz65xgjftjfhnh4
4threeeightsevenjsltzglonesevenzhhhcdbjfive
one7nnxzvhmvl
dcmsrd4sevenfour51five2
qvrgbjkgfpdbndpdgdptjqdhqkcrjknfxbgn4
p2
3311dczzvrskxksevenpgxthreeeight
tworbzmdd45foursevenfive
5fivefive7trtdlnppfournine3
5sixvtbfcbfkvbvbv3986
six64sixltlpnvt77
1oneeighttwo
tjsgchgninexrszjtzseven86
3cnine18
gntwone89glp31oneseven
9twofrqprtrhklhq39fjdtnnnjconejrqbgrdhhj
twozvmhxrhjzlhfninerbhzjxrgnmjt7
6five6fivezsbjjtvxvd9sixfiveqnsr
2threevldkrspksbsixsixtdgzmjrs2
fourhlstxkvr74qnx
one83
kdpdcvjlcsevensevennzvfourpvmlgttbl5
93smjxjpgnp8
dfmjhmrconetbztgfgf8six3hlbqdzqqs
4twotktfive
7nine1fscqqcml2two1fhddkblmgrtkrn
6fivefoureightnineninefive
39fourlsnmh7one
3fhkbpdmmgkgf
eightone3qjpfddljjjpjjmcg2twonexh
4two2517
seven6sixvrhjm543ckq
four7ninethreeonevfqrkhtx4
jeightwo5
fnndfvpc3five3h5
xkqstv5jpfonetwogjlbx5
fiveone8eightfive
2twomjkthreetkxtht2one
seven65gbmjcsmkr3
3qtrdtwo52eightntlbdmqcvseven
4cghqgpsmjztwo
8tqjlbpjmtwolrxp
ggrxfxrx15
3vt8sevenfourbkgcbvll28
lmgzcd4sixslonetwo
ptstwone6mpspfsevenh765eight
jfgpcfrgrmfnine5
fivec1ninefivefour9kmthmtwo
5zscbtpbbbbkhjrlxtnf4cmbfhbtwo
2mmgv
zmhcsixeighttwonine5gvljrkvqh
4five3
1four8six2one
sevenseven26mnbd7nine
vmxrptvk964sevendmdsql5
sevenfivekllbkzrmpmeight2twothree4
one45kjlnmznt67bgpl
eight7gpc6vvzqone
hjdbghreightthreesixzphtmj81
5dqftptqnfthree2nine5
86sixqgmlbnpnsixbxnfhmcdt8threefklhnsmkz
5vjgqnvvlonevxjzh9vctwo7nine
63ffslvtvvtgkmhvf1ntjtseven
fivefivev5tbpkx2
fourrsb5eightsfqsblmkkeightbnpqkmxxc6five
79kddeight
j5qltpmnineseventwo5
54gbmmmrfour
6ctfvjtnvdfourxjkjseven
1frq
75xhz7one4xlb
onetvxfive221zj2
sevenvkxvmqd93
5nfnldqtxpb35seven8
twobxtrpdzvbktrh1seven2two8
qsixeighttwofrxbxfspqmktpj5
2twoseven29knktthzssnhhf
595eightfiveeight4
rzmktrreight6n5seveneight
fivesix9
kqc81one
t271fivefournine5
65threefiveqfsmone
brhlq1eight931fourtwo
nine7eight
68nsgrvk2six
419gsmxnmjfour5
jqrh848
2eight91
sfgzxpbhb1cxjpd5ninegvpfmhkfgjkgblseven
dmnrmp77qqnbpvzeighttwo
pmgjdtdfx6
prrzx4jfjzpcklsb5sevenfour7six
brsqmz38five5twonebp
seven4threefour
18gmfvqgteight
7oneeightfourseven
onepkfnnvjpkgbvt8
fourkj4onenlkjcrvl
1phb5
16thgsmdqbnfivethreehbmmninenine
9qfqhdgthreeqmdg9cjzkjckfttwo
dxsevenone1mxmcfkgtdsczbgsx8
9jmhxphrhrjqmnnine3rxfmhjjqt
fivevjrninethreeseven6eight4
gltmpctwopcbchlj9r91qbdvsz
nmskpljnffbtwo7ffive5kqr
hlceightwotwoctsix4kkppmcxh
7vdtbxbsdm9nine9mqbgzgb6
threethreeeight4lfshmkpsbrveightsix
s77eighttwo
kxgntkmxshkllhctzk14
eight9lks3574
66fourstxmfqzpseven8p
46nine1twocqcqzn
vvzmqcj5fiverrj3threepltjs
2blshrhbpsnqqgxtnlzthreefour
4bqcssvxmxtwoeight4threeonenine
onezphmrtxbtlczz5eight
qcvvlc92mhhqpfsrrfndgl
seventwo5
2eight5oneeighteighteight32
cbmbfsztjnine3ztzbk49lvvlknvrnn
4qbtglxjhzb
5qzrlv4nxlxzbcdjsevenseven2jphvkthb
seven56
fivejlpnftwo7nz4fiveone
53hszlseven
tzr5sevenone5
ninegmzppseven7
dhrcgzqctwoeight8qhtlpf5jhvbcl
98onetwo715tcg5
tlfoursix5gzbntsevenseven
eight7peightonetwo937
6djvpnxvgfvbzpbgh5fk
ktxcqsgjthreesevenjrsgddzrfour8
lqmvrqgtdcmtzprq9threefour11jjkffcd
kbnlmsixfivetwohxftxpmvfn7
1qhf239nine
125zdjtsdxmthreeeight22eight
37
five827
lzn25
cspdsgf41nsjlthreedpcmq1nine
flvgklrtwodpxk2twobg57
8seven5nine
82seventpqmczz28fiveone
854vtxbrr2
three6seven6sevenonejsqtjpt
sixzbsxxngxcninesix82
mrbsp8sevenjhsfxgvpbv8
nine9kh
6fhzkrxsxthreeone
tvpmseven3fthlkndskdgfhjrvcxcninetdlkgrnxpm
tbrgbllqffourfqthree2
1sixvtmjtwokb71bddxpv
twolsjbscrfjzjsnphgsbthsmch6two5
3fouroneeightvbxsixb2
41dbqvfthree
tjjtpjpg1
6rfmfouronethreezgjsprtmd1sixeight
six182tbjjx7
4ffsgbshfzq93five
4threecncqtdklqt
nrkkgxzlsevenrqhbvzfvn99oneqjlxld9
xchnxhmfhpfiveblccdzdnqbzbm8gjrbqdslbbhdxpbpm
hbfdlrxmkrbc9threeonefive1rrvlfgjrq4
1eightthreeclpnzrtfive2
4nine1lsmmjjhsbninehlfxbbmsjcone
four16seven3fhvhlhcrmone
qszfive3sixbcdkcmdqhj3six
eightonelmbjb1cfx
3three1sixsix7
1cnmrhxhzrkshqnpqmfour8fqlfzq
two9sixbcgpv2rnnkdpeightthreenine
vbzqpkmqtpbfourseven6bhzghsix6
fhtwone26sixtwothreeseven
jvoneight7eightdfvljxthreethreefoureightkvb6
6hrnfhcn2nlflcj
gqktqlbkbeightninehvfql3lbfllrnrblqchfmn6pknq
twosix4nine
36sevenzlzgftkzznxqpnrbhtwoeightfive
qxqckssnrd8sdprgknine
1seventwoseven
mnfhgnrlfzfive1
pghfiveeightseven4nfzlkgdcvk
jvcmsdlfeight4
dtmt1sixkgmvnfvqd1sjvslttcfmkgngkrmbsmd
gltskr21
vvqoneightmfkbp61qkdpsvlhqctfseventhree
qvggksmf1
xhmzv926sixlqtwoeight
647six51
5qbx5seven8oneeight
mmcktfzstwoblzthssjnine1
558
48threenine4hlfdjnxdpnine8
hsghqrmpzbqsl7flbcjqqmrxkkcghdzmthreesix
27tldnbdsixtwosevennjdjxz
sevensixsbphqrqtqs9983
7eightsixzrktkone
91npjhccblmpxtslz78kcvqtvqpmone
tnshjg21twothreeonesxkpgctcjdjpxt
six9twothreeqx4one6one
ninenine6npnszkvcgn
four5threenineone6
4twobgdhnbtcssevenmmzfgktwo4
three38
ndfplbcdsnrhgpq1two4
xhdfzl78twofivethree
zqcccjkgrdnr5cxmfhthreeskzcqkczp
vltlfh1qfdzgp8five
mqjnsx99ninetwo8
bnk7
dqk62ninegvhbmclnrfztmtfvrzjrv
85dnine3lkzmn
onesixthreekdrgmlnddsc7
sds6
9vkhr49sixxgqbgmsheightfive6
onetwoxrhzg1ninefourseven32
fsthreeczsqjcjlxqxgxrbfour8nine
threeseven576vrvj7d7eightwogc
6rfpeightfive4zsrnsvvbdd
3fiver6nnpmgmhzlhzfxb16jg
three9qhlpninethreethreemfr7
rx6dhk1sjrpmpjr
three4nhvkksevenxzbzbflnrcmmzg
eightvckzjgonetkcvjzdhnzfoursix6
onetwothreednktwoseight7jkpsgtdllf
nx6eight13nine
qzvnvsjnfjng99threesevenvc
seven2eightbpzsix
onefour8xccmksgfsckxkbcsqkgdgszxjgllh
8zfour
fiveeightrssqtwo992
8nbvcfjzpxnpfour4
zhqmxqlzv6onetwoeightkrfg25two
9tqbeightfmgqtdbl
three3nine97
vpcmmlctgc6
3two4fhnh2ninesixmtfbbvdkls6
171
lveightwofive8ninetwo3sixzp
3oneqhgonerjskld3eight5
zfxnkhrthreexllntwo1two9fivetwo
sixfivexmxxq7
xxhthqxnjq3tgjdvrnqtgts9
7226fiveqzbvgttlmv
884xxxt9
781
4dtpcdslmg9mdxsjksixnine
onesevenkqzg2
nine4nine
cdqmrvdkxtrqdsixnvqxs4fiveninefvdttbqkx
blcbcgbrnkpmlrdlj7fivedbnkkleightone94
1hgtmpdlpsfivethree8four5
vx93czsixkzkjpqbgzhthreetwo
zfbxmsjrmssixninebkvseventl69
jcnkdqmdvg392
pqkgqzgtwo245two9
1zzjc7onejdf5snc
25dhcpqbkzjxcxmxdgxftj
kzqsix8xblzlnjtvbfive5four35
ghfjjhphnftwo9five
jqstpxv5hcqfbh7twotwofive
jj66nsixhfsbgs6kmdpqdrpn
xfvc73snqtcpqcjlsix6526
tbpklzqjcz6fttcvqd
9pdrzfvrr9twolzgs
fiveseventhreeszxkxjx9ndvtlhqqqh7spfcvpzthree
68blhzlngj7
hznljt4eight8qfkhmqvpskcxknz9
eightjqlghmn76two5ggone
onesixmgbfjv41fhoneightvxl
jjlthreenine839
twotwo4xvlpcnpmtwo2jrzcjjlfteighteightwotv
t4
onefivefivefour94eightfour
6sixtj6threethree2sevenone
1eightkbsixrhhphnxmjlf
mlplpjkndlflk1nineninetndsqjnpmvzhkeight
jmkqqblqnxfivetwo8485eightone
five9565three3nineseven
bmfljlbbttxlvxzrfnnp319six
jb4one96
jqllbjqndlkbxkeightdrbhjjd3
8fiveggdtrfjvrpd7six7
24xrt3twosix2
rccf47zzmfshdtwosrsm
5jtqxkpshnbtwohtpbgtgsixone
seven77p
eightqgkmdvhdjssqfpfnk3sixthreesixthree
9ninetwoonelkrspqn3
4ttdgvkghzlninevkrxhclmstdq9mpmgtcvrlqvlqgtrxb
two7twojxzbrhtnpfrhpvsevenfive
4threetbkddx252tmspeight
five9xbcrfrxbdnb
threetwo6seven2foursix
onekeightxdgxjmthreeszrjhmk9
6q565cvdhbjldkb
two1three
dzrhvfktjninepfxvxmzmkeight1
xzbgjqfour5nphmrhlfdkk
38rfjeight5
848bfztdm
rldlk2zbmpsl8
gqgtrchtwoeight9eightone
27f
btgxvnm8seven2
sixmznpvvsbrslmtgfpvctz96hrjdlclnzmftjjdmp3
hsppfkeightsevenfive2
2two35kxssnjs21three
3ninefivemjslrzzsnl
9lzdmtdeightone3smfpmlvs5zkxmrssq
zrgsrbgcrllnjfrk9eightsevenlflnkxfgbzm
ninexsgvvslvvczfshvgfour6ngkkmcrkgbsngkzvcch
26six997
1zbsth
psdksevenone776
one8lzcrggjfrjssixkjtwo
sevenfour5three91six
brg3ninetwo5rvnpqsix
9seven9sixssnfourxsdbgbrpvxcmqh
6cmcheight8
onefive1sevenfournine1
threej3
lmzq27kkzxbtwo142
krhhmmsrklseven5pmjpthree8
lgplxpx7eighttlvqdmmlhz9gjfbxnlkkg
tfrpbxpsbghffsixrgm4
8fjnflxr2613
7onevzqcgqsevenrgs3twonex
8nine9brsnqlvtfivefour4fourtknbsm
grct7svvdbqtsrxsix9
544
bldknmxccsix4
xjdxzmhxqbfive36frtstwothree
4two7nine
8five99jddntnhjjsxjgfsqhljcrq
twotwofivethreeseven4tzgnqcq
sevenlvxjjrsjzvjxsltlfour2qmqtbklpkp
sixvrlseven1twothree
fivepcrhdtkrfour2snvjhhqzl8cbfkgghvb7
mhhfjmv4nine6rvdvhjlfgsixjzghsdf5
7fgmnvtfn94qbtrfb26
threeonejxjkcz2
1eighteightfour9xxdqnine
3threefour932
mtnp6onefour4oneseven3cjcm
fivetwo7four
sevenrtbx6trxfffsdlgztwo3
eight16fiveoneninejnpvnglzjd
7pqvfjeight
two87sbseven88
73678seven4three
pmxdvndvsixcvnntjcv4sevenhfmklmztwoeight
pdplkff87fouronefqrrbnine8djkgh
eight35vlllkzmnlpcgngjnt
897kqjhkfhfbone
ch6
5threefivehfnvsphfxqdltgtbjtwofour3
nine5fourfivejtrhmnbmx
eightqthree5gdpvnt2eightpbd
jbv34nine
lcqxvbhr1
4dpcbxj
seven3eight75
75ronenine4four
4ppfnkjpphppsjr4
2cjkjnvnjxtwolhmnrlcpzmbkhzjkcvknczjvcxj
tj5rgmvvhmc2
three7scghxlnbsmgtjgqqtmxrfnvtlvcpp16
fiveonefourxlqcttwosnine9
323hmpxbkpdvklcrnglmhdlcmmzdtwo
onevvbvctdqsvhqb9
4foursix3
56seveneight281five
5t1ldfbstvrvbks2sevengfndh
6four48
4fmgjxzxhq
sixkmltvqjv6qfpf235
three4djpcgvbone1bztqnhk
5fourfjtflgsixtwofourthree
4nine9gcdnfjm
xfcnmpbvmmhkfiveseven88v6
68sixtworztsbzvghjsmchmsxmdnine
seven1eight
3threesxjqxxx2threeeight2
twontnxvnjjtwocpxmrsix2x2nc
nineltgzzzzqg2sixnxdhcb36
623twohnmqptwo8
ntwonebtcqztrvninenine58glpkdpdghkbtrsz
rlnthmmdfsvmdfqhfivendptjfpx7hnbnkzlpntvglvdlfb4five
cmx6four6
81ninefourqbljfrbshsnmcqmzrhkznl
psixthree8jrmhcvpdgflsnnvdgmkxtxrl
2ggghpzxxlmrlftpqmsg
7four296nine
2xonesevenonenine39ctq
four915fdtcvcrv
fsssjgbncc8nine
qgpfveightdvtkhxxzrcmfivesix4
4436eightonefspnbptktv
rlmmlrhmjnfivetwo6sixmxs
443ninesixfour
fivexmcdmpxfcn8ddnrhbl9znqxtf
djtkclphr4ninesixfiveqgksrzj6nineeightwogz
eight494dd
klmmxtlvxsnineeightrxzzksjxcvgvznonevbnhxjlrxh85
vftdg4fvlseven9fnh
rglcjdkcsevenrvrkrrptx4mfbqb
767five9
eight1four3
fourfived84threethreeeight
two5foureight
seven43gzzmksixhmln2
bjknl46ninexrjbr5fourfour
2pns7fpjznxkponepfive
sixkqdhm1ninetwor7
threefive8
two8lhlphf
seven3seventwo
4psqnsmqgz25
eight33fiveg5oneeightwoh
sixsixxldfive8
qhljssjbsevengfh5seven
gjsqptqkjk3bhj6five
six6szlss9seven
nmzxrgthreemtckbvglb7six
ffxmfive392fivethreenineseven
sevenvvnssgvrq8ninedfvqzgrdtqnh
hbsgz8nineczksxzllx2five
9twonine
4fz
3fqjnpxnvseventhree7ninefive9
phlvsix123pszgrxvhjkk
3two4
klcbnvrfv31zbb236
eightseven1lkcv1six9
7four7fiveeight
lqninenmqmbsgk9
sevensixmczghz299six
49sixlcbtsbnineseven9five
rtwone36sblhninemflhd
8nineblqjzsddone24dnfdgph
jzlbhlhvfour4dlvctlrqsix
threeseventhreenlhcc8
2xgccjbmdlzlstjvqsknd
97ninedfcmx9359l
7sevenqzvlfqlfsdxsix
8682
2bhzhzpglp
sevenzbfvlpn7tkhmxtvgvfeightwobsb
npdfzsevenonezc1two
2hcmrldtn5
threeseven286fourfour
seventwo5eight
ninefourmqxjdthczlqp4xrxrptzvdllctwo
ninefour8
q52xzpxvkgsixctrjg
6four52qkvtqpqcfivevghjmltckgtjjf
qxxkgqqcggbjc85
lninelbzqtdbnvvmchxrpsbfivehtmqcc3
xbtlshzqtrtwo8sqffcsfivetwo3mvxsnkgseven
5pqlfrqqhpgtg6four
6lxjzthree14six4
three22fxrrdvk
sixztwoeightnine4seven1four
fivegbvrtkvldplfzrvxbmkmrcftxs4oneqhmxseventhree
threenine2vzdkxtz7
7mnmfkfq48
four66grzkq86six
four3threezdszkzkbhnsqpmsninebq
746
55fiveqhvtgfz
c2mjsrdzmtsevenfive6ncjrvzc4
ntwoneeight2five93jskvfcvn
fvzjvxtwozhnhnmqv3seventhree
2three5three
2sevenclone1
three9two4kqxq7four
eight8gzhcfbjxjnftxnnjl
mkz4two7fivefive9fivethree
eighttwo9seven6
eightkdpfprqdz6
9twofczb
sqmhjgzqvc3
525three9jnppn
eightkpfngjsx97twozmbdtxhh
snc6
two3dqpnps3pmdnbxdnlc8
pfnv2
four1sixeightm35
sixldzmvtfsthreeseven1ninethreeseveneight
7jrxgdjfh3xvlpfgckjp6fourfive
sktmpngljrrgvpqqkdnine14d
one79
1five7jmhtbnkvcg8vgrxdbnr5four
fvzcslpmgv174426
3nineccslpsrfdf35lqbfqbncs83
364twoqvqr4
79lfd2nineeight2ghrlbspvkzseven
nine1ztqbs
eightndxxqxtwo3cqz47
fiveeight792eightqskstrftdpccsrgskrhc
26fmrrhhpthree6b)";
*/