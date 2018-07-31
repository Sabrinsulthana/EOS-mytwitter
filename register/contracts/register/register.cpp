#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>

using namespace eosio;
using namespace std;
using std::string;

class twitter : public contract
{

public :
        using contract :: contract;


	/* struct for login */
	///@abi table login i64

	struct login
	{
		account_name lname;
		string lpassword;
		uint64_t primary_key() const {return lname; }
		EOSLIB_SERIALIZE(login,(lname)(lpassword))
	};
	typedef multi_index<N(login),login>log;


	/* struct for registration */
        /// @abi table regis i64

        struct regis
        {
                account_name ac_name;
		string pass;
		string s_key;
		uint64_t primary_key() const { return ac_name; }

                EOSLIB_SERIALIZE(regis,(ac_name)(pass)(s_key))
        };

        typedef multi_index<N(regis),regis> reg;

	/* struct for chatting */
	///@abi table chat i64

	struct chat
	{
		account_name name;
		std::vector<string> msg;
		std::vector<uint64_t> time;
		uint64_t primary_key() const {return name; }

		EOSLIB_SERIALIZE(chat,(name)(msg)(time));
	};

	typedef multi_index<N(chat),chat> cht;

	/* personal chat sender */
	///@abi table sender i64

	struct sender
	{

		account_name sname;
		std::vector<string>smsg;
		std::vector<uint64_t>stime;

		uint64_t primary_key() const {return sname; }

		EOSLIB_SERIALIZE(sender,(sname)(smsg)(stime));
	};

	typedef multi_index<N(sender),sender>sdr;


	/* personal chat receiver */
	///@abi table receiver i64

	struct receiver
	{
		account_name rname;
		std::vector<string>rmsg;
		std::vector<uint64_t>rtime;
		uint64_t primary_key() const {return rname; }

		EOSLIB_SERIALIZE(receiver,(rname)(rmsg)(rtime));
	};

	typedef multi_index<N(receiver),receiver> recve;

	/* follower,unfollower struct functionality */
	///@abi table follower i64

	struct follower
	{
		account_name name;
		std::vector<account_name>fname;

		uint64_t primary_key() const {return name; }

		EOSLIB_SERIALIZE(follower,(name)(fname));

	};
		typedef multi_index<N(follower),follower>flwr;


	/* struct following */
	///@abi table following i64

	struct following
		{

		account_name sabfol;
		std::vector<account_name>tofollow;

		uint64_t primary_key() const {return sabfol; }

		EOSLIB_SERIALIZE(following,(sabfol)(tofollow));

		};

		typedef multi_index <N(following),following> flwng;


	/* struct post like */
	///@abi table postlike i64


	struct postlike
	{
		account_name pname;
		uint64_t count;
		std::vector<account_name>plname;
		uint64_t primary_key() const {return pname; }

		EOSLIB_SERIALIZE(postlike,(pname)(plname)(count));
	};
	typedef multi_index <N(postlike),postlike> plke;


	/* struct dislike */
	///@abi table dislike i64

	struct dislike
	{
		account_name dname;
		uint64_t count;
		std::vector<account_name>dlname;

		uint64_t primary_key() const {return dname; }

		EOSLIB_SERIALIZE(dislike,(dname)(count)(dlname));
	};
	typedef multi_index <N(dislike),dislike> dlke;


	/* struct for share */
	///@abi table share i64

	struct share
	{

		account_name shname;
		uint64_t count;
		std::vector<string>shmsg;
		std::vector<account_name>psname;

		uint64_t primary_key() const {return shname; }
		EOSLIB_SERIALIZE(share,(shname)(count)(psname)(shmsg));
	};
	typedef multi_index <N(share),share>shre;


	 /*  for twitter registration  */

	///@abi action

	void treg(account_name name,string pass,string s_key)
	{
		reg twit(_self,_self);

		auto i = twit.find(name);

		if(i == twit.end())
		{
			twit.emplace(_self,[&](auto& sabrin)
			{
				sabrin.ac_name = name;
				sabrin.pass = pass;
				sabrin.s_key = s_key;
			});

		}
		reg twit1(_self,_self);

		auto i1 = twit.find(name);

		if(i1 == twit.end())
		{
			twit.emplace(_self,[&](auto& msg)

			{
			msg.ac_name=name;
			});

		}

	}


	/*for password modification*/

	///@abi action

	void tpmodi(account_name name,string pass,string s_key)
	{
	 	reg m(_self,_self);

		auto i = m.find(name);

		if(i != m.end())
			{

			m.modify(i,_self,[&](auto& tpm)

				{

				if(tpm.ac_name == name && tpm.s_key == s_key)
                                	{
						tpm.pass=pass;
 					}

				});
			}
	}


	/* for  chatting */
	///@abi action

	void chatt(account_name sname,string msg)
	{
		cht c(_self,_self);

		auto i= c.find(sname);

		if(i == c.end())

		{
			print("lets start chat when you are logged in");

		}
		else
		{
		c.modify(i,_self,[&](auto& cs)

		{
			cs.msg.push_back(msg);
			cs.time.push_back(now());


		});


		}

	}

	/* personal chat sender */
	///@abi action

	void  srmsg(account_name ssname,account_name rrname,string msg)
	{
		reg s(_self,_self);

		auto ii = s.find(ssname);

		if(ii == s.end())

			{
			print("msg has been sent");

			}

		sdr ss(_self,_self);

		auto i = ss.find(ssname);

		if(i == ss.end())

			{
			print("need to create an account");
			}
		else
			{
			ss.modify(i,_self,[&](auto& snd)

			{
				snd.sname=ssname;
				snd.smsg.push_back(msg);
				snd.stime.push_back(now());

			});


			}

		recve rr(_self,_self);

		auto i1=rr.find(rrname);

		if(i1 == rr.end())
			{
			print("msg has been received");
			}
		else
			{

			rr.modify(i1,_self,[&](auto& recvr)
			{
				recvr.rname=rrname;
				recvr.rmsg.push_back(msg);
				recvr.rtime.push_back(now());

			});


			}
	}


	/* follower function  */
	///@abi action

	void follow(account_name name,account_name fname)
		{

		reg f(_self,_self);

		auto ii= f.find(name);

		if(ii == f.end())
			{
			print("you need to create an account");
			}

		flwr ff(_self,_self);

		auto i=ff.find(name);

		if(i == ff.end())
			{
				ff.modify(i,_self,[&](auto& flr)

				{
					flr.name=name;
					flr.fname.push_back(fname);

				});



			}


		}


	/*unfollower  function */
	///@abi action

	void unfollow(account_name unname,account_name ufname)

	{

		reg f(_self,_self);

		auto i = f.find(unname);

		if( i == f.end())

			{

			print("you are going to unfollow");

			}

		flwr uf(_self,_self);

		auto i1 = uf.find(unname);
		auto i2 = uf.get(unname);

		if( i1 != uf.end())
			{

			for(uint64_t i = 0; i < i2.fname.size(); i++)
				{

				if(ufname == i2.fname.at(i))

					{

					uf.modify(i1,_self,[&](auto& uflw)

						{

						uflw.fname.erase( uflw.fname.begin() + i);

						});


					}

				}

			}

	}

	/* following function */
	///@abi action

	void follwng(account_name name,account_name fname)
	{
	 	flwng fg(_self,_self);

	        auto i = fg.find(name);

		if(i == fg.end())
			{
			print("you need to create an account");

			}

		flwng flg(_self,_self);

		auto ii=flg.find(name);

		if(i != flg.end())
			{
			print("you need to create an account");

			}
		flwng fwg(_self,_self);

		auto i1=fwg.find(name);

		if(i1 == fwg.end())
			{

			fwg.modify(i1,_self,[&](auto& sabfolwng)
				{

				sabfolwng.sabfol=name;
				sabfolwng.tofollow.push_back(name);

				});
			}


	}


	/* unfollowing function*/
	///@abi action

	void  unfollowing(account_name name,account_name namef)
	{

		flwng ufng(_self,_self);

		auto i = ufng.find(name);

		if(i == ufng.end())
			{
				print("you need to create an account");

			}

		flwng ufn(_self,_self);

		auto i1 = ufn.find(name);

		auto i2 = ufn.get(name);

		if(i != ufn.end())
			{

			 ufn.modify(i1,_self,[&](auto& unfng)
					{

					for(uint64_t i=0; i < i2.tofollow.size() ; i++ )

					{
						if(namef == i2.tofollow.at(i))
						{

						unfng.tofollow.erase(unfng.tofollow.begin() + i );

						 }


					}


					});

			}


	}


	/* post function */
	///@abi action

	void pstlke(account_name pname,uint64_t time, account_name plname,uint64_t count)
	{

		reg p(_self,_self);

		auto i = p.find(pname);

		if(i == p.end())

			{

			print("you need to create an eos account");

			}

		cht pp(_self,_self);

		auto i1 =  pp.find(pname);

		auto i2 = pp.get(pname);

		if(i1 == pp.end())
			{
			   print("you need to create an account");

			}
		else 
		{

		plke pl(_self,_self);

		auto ii = pl.find(pname);

		if(ii != pl.end())
			{
				for(uint64_t i=0; i<i2.time.size();i++)
				{
				        if(i2.time.at(i))

					{
					auto ii =pl.get(plname);

					for(uint64_t j=0; j < i2.time.size(); i++)
					{
						if(i2.time.at(i))
						{

						pl.modify(ii,_self,[&](auto& like)

						{
							like.plname.push_back(plname);
							like.count += 1;

						});

						}

					else
						{

						pl.modify(ii,_self,[&](auto& like)
							{

							like.plname.erase(like.plname.begin() + i);
							like.count -=1;

							});

						}
					}
					}

				}
			}

		}

	}

	/* postdislike function*/
	///@abi action

	void pstdl(account_name dname,account_name dlname,uint64_t count,uint64_t time)
	{
		reg pds(_self,_self);

		auto i = pds.find(dname);

		if(i == pds.end())

			{
			print("you have to logged in ur account");
			}
		cht pd(_self,_self);

		auto i3 = pd.find(dname);

		auto i2 = pd.get(dname);

		if(i3 == pd.end())

			{
			print("you have to logged in ur account");
			}
		else
			{

			dlke pdl(_self,_self);

			auto ii=pdl.find(dlname);

			if(ii != pdl.end())
			{

			for(uint64_t i=0; i<i2.time.size(); i++)
   				{

				if(i2.time.at(i))

				{
					auto ii = pdl.get(dlname);

					for(uint64_t m=0; m<i2.time.size(); i++)

						{
							if(i2.time.at(i))
									{
									pdl.modify(ii,_self,[&](auto& dislike)
										{
										dislike.dlname.push_back(dlname);
										dislike.count+=1;
										});
									}
								else
									{
										pdl.modify(ii,_self,[&](auto& dislike)
										{
						 				dislike.dlname.erase(dislike.dlname.begin() + i);
										dislike.count -=1;
										});
 									}

						}

				}

				}

		}


	}

	}

	/* share action */
	///@abi action

	void sha(account_name shname,uint64_t count)

	{

	reg shr(_self,_self);

	auto i = shr.find(shname);

	if( i == shr.end())

		{

		print("you have to login to share your post");
		}

	}


	/*post share function*/
	///@abi table

	void postshare(account_name shname,uint64_t count,string msg,account_name psname)
	{
		reg pshr(_self,_self);

		auto i4 = pshr.find(shname);

		if( i4 == pshr.end())

			{
			print("you looged to share post through this app");
			}

		shre psha(_self,_self);

		auto ii = psha.find(psname);

		if(ii != psha.end())
			{
			psha.modify(ii,_self,[&](auto& psh)
				{

				psh.psname.push_back(psname);
				psh.count +=1;

				});

			}
		else
			{

			print("you are now logged on");

			}

		}


	/* login twitter */
	///@abi action

	void loginn(account_name name,string lpassword,account_name lname)
	{

	reg lg(_self,_self);

	auto i = lg.find(name);

	if(i == lg.end())

		{
			print("you need to create an account");

		}
	log lgg(_self,_self);

	auto i9 =lgg.find(name);

	auto ii1 = lgg.get(name);

	if(i9 != lgg.end())

		{
		lgg.modify(i9,_self,[&](auto& llg)
				{

				llg.lname=lname;
				llg.lpassword=lpassword;

				});

		}
	else
		{
			print("you are looged in ur account");
		}

	}

	/* account delete */
	///@abi action

	void del(account_name user,string password)
	{

	cht delt(_self,_self);

	auto i6 = delt.find(user);

	if(i6 != delt.end())
		{
			delt.erase(i6);
			print(" you deleted the data");

		}

	else
		{

			print("Username ", name{user} ," deleted the data ");

		}



	}



};




EOSIO_ABI(twitter,(treg)(tpmodi)(chatt)(srmsg)(follow)(unfollow)(follwng)(unfollowing)(pstlke)(pstdl)(sha)(postshare)(del)(loginn));

