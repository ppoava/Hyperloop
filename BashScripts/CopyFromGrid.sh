#!/bin/bash
# Load O2 env before running this script
#TARGET_DIR=$1
LOCAL_DIR=$1
#RUN=$3
#PASS=$4

# NEW_DIR=$PASS
# mkdir -p $LOCAL_DIR/$NEW_DIR
TARGET_LIST=( /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746239 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746238 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746237 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746236 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746235 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746233 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746232 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746231 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746230 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746229 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746228 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746226 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746225 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746224 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746223 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746222 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746221 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746220 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746219 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746218 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746217 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746216 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746215 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746214 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746213 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746212 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746210 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746209 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746208 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746207 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746206 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746205 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746204 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746203 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746202 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746201 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746200 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746199 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746198 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746197 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746196 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746195 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746194 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746193 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746192 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746191 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746190 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746189 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746188 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746187 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746186 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746185 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746184 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746183 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746182 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746181 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746180 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746179 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746178 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746177 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746176 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746175 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746174 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746173 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746172 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746171 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746170 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746169 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746168 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746167 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746166 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746165 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746164 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746163 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746162 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746161 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746160 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746159 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746158 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746157 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746156 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746155 /alice/cern.ch/user/a/alihyperloop/jobs/0074/hy_746154 )
LOCAL_LIST=( 545367 545345 545332 545312 545311 545295 545294 545291 545289 545262 545249 545223 545222 545210 545185 545184 545171 545117 545103 545086 545066 545064 545063 545062 545060 545047 545042 545041 545010 545009 545008 545004 544992 544991 544968 544964 544963 544961 544947 544931 544917 544914 544913 544911 544896 544887 544886 544868 544813 544797 544795 544794 544767 544754 544742 544739 544696 544694 544693 544692 544674 544672 544653 544652 544640 544614 544585 544583 544582 544580 544568 544567 544565 544564 544551 544550 544549 544548 544518 544515 544514 544512 544511 )

for i in {0..82}
do
	cd $LOCAL_DIR/
	mkdir -p ${LOCAL_LIST[$i]}
	cd $LOCAL_DIR/${LOCAL_LIST[$i]}
	echo "Start processing ${TARGET_LIST[$i]}"

	CTF_LIST=`alien_ls -l ${TARGET_LIST[$i]} | grep "^d" | awk -F" " '{print $8}'`
	echo "Found jobs:"
	echo $CTF_LIST
	for CTF in $CTF_LIST
	do
		mkdir -p $CTF
		echo "Processing $CTF"

		for ITEM in AnalysisResults.root
		do
			echo "Copying $ITEM ..."
			alien_cp alien:${TARGET_LIST[$i]}/$CTF/$ITEM file:$LOCAL_DIR/${LOCAL_LIST[$i]}/$CTF/$ITEM
			echo "$ITEM copied."
		done
	done
done

cd $LOCAL_DIR/
# hadd -f mchtracks.root o2_ctf*/mchtracks.root
# hadd -f muontracks.root o2_ctf*/muontracks.root
# hadd -f mid-reco.root o2_ctf*/mid-reco.root