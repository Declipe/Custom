#include "Log.h"
#include "AccountMgr.h"
#include "SpellAuras.h"
#include "Config.h"
#include "GuildMgr.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "WorldPacket.h"
#include "ObjectMgr.h"
#include "ArenaTeam.h"
#include "ArenaTeamMgr.h"
#include "World.h"
#include "WorldSession.h"
#include "Group.h"
#include "AchievementMgr.h"
#include "ObjectAccessor.h"
#include "Unit.h"
#include "SharedDefines.h"
#include "Creature.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Cell.h"
#include "CellImpl.h"
#include "Language.h"
#include "Chat.h"
#include <sstream>
#include "Channel.h"
//#include "AuctionHouseMgr.h"

class premium_commandscript : public CommandScript
{
public:
    premium_commandscript() : CommandScript("premium_commandscript") { }

    std::vector<ChatCommand> GetCommands() const
    {
        static std::vector<ChatCommand> premiumCommandTable =
        {
			//{ "add", SEC_ADMINISTRATOR, false, &HandleVipAddCommand, "" },
            { "bank",  SEC_PLAYER, false, &HandlePremiumBankCommand,    "" },
            { "mail",  SEC_PLAYER, false, &HandlePremiumMailCommand,    "" },
			{ "buffs", SEC_PLAYER, false, &HandleVipbuffsCommand, "" },
			{ "arena", SEC_PLAYER, false, &HandleVipjoinArenaCommand, "" },
			{ "warsong", SEC_PLAYER, false, &HandleVipjoinWarsongCommand, "" },
			{ "arathi", SEC_PLAYER, false, &HandleVipjoinArathiCommand, "" },
			{ "eye", SEC_PLAYER, false, &HandleVipjoinEyeCommand, "" },
			{ "alterac", SEC_PLAYER, false, &HandleVipjoinAlteracCommand, "" },
			{ "debuff", SEC_PLAYER, false, &HandleVipDebuffCommand, "" },
			{ "map", SEC_PLAYER, false, &HandleVipMapCommand, "" },
			{ "resettalents", SEC_PLAYER, false, &HandleVipResetTalentsCommand, "" },
			{ "repair", SEC_PLAYER, false, &HandleVipRepairCommand, "" },
			{ "capital", SEC_PLAYER, false, &HandleVipCapitalCommand, "" },
			//{ "status", SEC_ADMINISTRATOR, false, &HandleVipStatusCommand, "" },
            //{ "auc",  SEC_PLAYER,  false, &HandlePremiumAuctionCommand, "", NULL },
        };

        static std::vector<ChatCommand> commandTable =
        {
            { "vip", SEC_PLAYER, false, NULL, "", premiumCommandTable },
        };

        return commandTable;
    }

    static bool HandlePremiumBankCommand(ChatHandler* handler, char const* /*args*/)
    {
        Player *player = handler->GetSession()->GetPlayer();
        if (player->GetSession()->IsPremium() && sWorld->getBoolConfig(COMMAND_BANK_PREMIUM))
        {
            //Different Checks
            if (player->IsInCombat() || player->IsInFlight() || player->GetMap()->IsBattlegroundOrArena() || player->HasStealthAura() || player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || player->isDead())
            {
                handler->SendSysMessage(LANG_PREMIUM_CANT_DO);
                handler->SetSentErrorMessage(true);
                return false;
            }

            handler->GetSession()->SendShowBank(handler->GetSession()->GetPlayer()->GetGUID());
        }
        return true;
    }

	static bool HandleVipResetTalentsCommand(ChatHandler* handler, const char* /*args*/)
	{
		Player *chr = handler->GetSession()->GetPlayer();

		//Different Checks
		if (chr->IsInCombat() || chr->IsInFlight() || chr->GetMap()->IsBattlegroundOrArena() || chr->HasStealthAura() || chr->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || chr->isDead())
		{
			//handler->SendSysMessage(VIP_CANT_DO);
			handler->SetSentErrorMessage(true);
			return false;
		}

		if (!chr->GetSession()->IsPremium() && !chr->IsGameMaster())
		{
			//handler->SendSysMessage(VIP_DONT_HAVE);
			handler->SetSentErrorMessage(true);
			return true;
		}

		// Reset Talents
		handler->GetSession()->GetPlayer()->ResetTalents(true);
		handler->GetSession()->GetPlayer()->SendTalentsInfoData(false);

		handler->PSendSysMessage(LANG_RESET_TALENTS_ONLINE, handler->GetNameLink(handler->GetSession()->GetPlayer()).c_str());
		return true;
	}
	//bag
    static bool HandlePremiumMailCommand(ChatHandler* handler, char const* /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();
        if (player->GetSession()->IsPremium() && sWorld->getBoolConfig(COMMAND_MAIL_PREMIUM))
        {
            //Different Checks
            if (player->IsInCombat() || player->IsInFlight() || player->GetMap()->IsBattlegroundOrArena() || player->HasStealthAura() || player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || player->isDead())
            {
                handler->SendSysMessage(LANG_PREMIUM_CANT_DO);
                handler->SetSentErrorMessage(true);
                return false;
            }

            handler->GetSession()->SendShowMailBox(player->GetGUID());
        }
        return true;
    }

    //static bool HandlePremiumAuctionCommand(ChatHandler* handler, char const* /*args*/)
    //{
        //Player* player = handler->GetSession()->GetPlayer();
        //if(player->GetSession()->IsPremium())
        //{
            //Different Checks
            //if(player->IsInCombat() || player->IsInFlight() || player->GetMap()->IsBattlegroundOrArena() || player->HasStealthAura() || player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || player->isDead())
            //{
            //    handler->SendSysMessage(LANG_PREMIUM_CANT_DO);
            //    handler->SetSentErrorMessage(true);
            //    return false;
            //}

            //handler->GetSession()->SendAuctionHello(player->GetGUID());
            //handler->GetSession()->SendAuctionHello(player->getFaction());

        //}
        //return true;
    //}

	static bool HandleVipjoinArathiCommand(ChatHandler* handler, const char* /*args*/)
	{
		Player *player = handler->GetSession()->GetPlayer();
		if (player->GetSession()->IsPremium())
		{
			BattlegroundTypeId bgTypeId = BATTLEGROUND_AB;
			handler->GetSession()->SendBattleGroundList(handler->GetSession()->GetPlayer()->GetGUID(), bgTypeId);
			//handler->PSendSysMessage(LANG_QUEUE_ARATHI);
		}
		return true;
	}

	static bool HandleVipjoinEyeCommand(ChatHandler* handler, const char* /*args*/)
	{
		Player *player = handler->GetSession()->GetPlayer();
		if (player->GetSession()->IsPremium())
		{
			BattlegroundTypeId bgTypeId = BATTLEGROUND_EY;
			handler->GetSession()->SendBattleGroundList(handler->GetSession()->GetPlayer()->GetGUID(), bgTypeId);
			//handler->PSendSysMessage(LANG_QUEUE_EYE);
		}
		return true;
	}

	static bool HandleVipjoinWarsongCommand(ChatHandler* handler, const char* /*args*/)
	{
		Player *player = handler->GetSession()->GetPlayer();
		if (player->GetSession()->IsPremium())
		{
			BattlegroundTypeId bgTypeId = BATTLEGROUND_WS;
			handler->GetSession()->SendBattleGroundList(handler->GetSession()->GetPlayer()->GetGUID(), bgTypeId);
			//handler->PSendSysMessage(LANG_QUEUE_WARSONG);
		}
		return true;
	}

	static bool HandleVipjoinAlteracCommand(ChatHandler* handler, const char* /*args*/)
	{
		Player *player = handler->GetSession()->GetPlayer();
		if (player->GetSession()->IsPremium())
		{
			BattlegroundTypeId bgTypeId = BATTLEGROUND_AV;
			handler->GetSession()->SendBattleGroundList(handler->GetSession()->GetPlayer()->GetGUID(), bgTypeId);
			//handler->PSendSysMessage(LANG_QUEUE_ALTERAC);
		}
		return true;
	}

	static bool HandleVipjoinArenaCommand(ChatHandler* handler, const char* /*args*/)
	{
		Player *player = handler->GetSession()->GetPlayer();
		if (player->GetSession()->IsPremium())
		{
			BattlegroundTypeId bgTypeId = BATTLEGROUND_AA;
			handler->GetSession()->SendBattleGroundList(handler->GetSession()->GetPlayer()->GetGUID(), bgTypeId);
			//handler->PSendSysMessage(LANG_QUEUE_ARENA);
		}
		return true;
	}
	static bool HandleVipbuffsCommand(ChatHandler* handler, const char* args)
	{
		Player *player = handler->GetSession()->GetPlayer();
		if (player->GetSession()->IsPremium())
		{
			if (player->IsInCombat())
			{
				//handler->PSendSysMessage(LANG_NOT_WHILE_FIGHTING);
				handler->SetSentErrorMessage(true);
				return false;
			}
			if (player->IsInFlight())
			{
				//handler->PSendSysMessage(LANG_NOT_WHILE_FLYING);
				handler->SetSentErrorMessage(true);
				return false;
			}
			if (player->GetMap()->IsBattleArena())
			{
				handler->PSendSysMessage("You can't use this in an arena.");
				handler->SetSentErrorMessage(true);
				return false;
			}

		{
			player->Dismount();
			player->RemoveAurasByType(SPELL_AURA_MOUNTED);
			player->AddAura(48161, player);              // Power Word: Fortitude
			player->AddAura(48073, player);              // Divine Spirit
			player->AddAura(20217, player);              // Blessing of Kings
			player->AddAura(48469, player);              // Mark of the wild
			player->AddAura(16609, player);              // Spirit of Zandalar
			player->AddAura(15366, player);              // Songflower Serenade
			player->AddAura(22888, player);              // Rallying Cry of the Dragonslayer
			player->AddAura(57399, player);              // Well Fed
			player->AddAura(17013, player);              // Agamaggan's Agility
			player->AddAura(16612, player);              // Agamaggan's Strength
			player->AddAura(24705, player);
			player->AddAura(26035, player);
			player->AddAura(31305, player);
			player->AddAura(36001, player);
			player->AddAura(70235, player);
			player->AddAura(70242, player);
			player->AddAura(70244, player);
			player->AddAura(30090, player);
			player->AddAura(30088, player);
			player->AddAura(30089, player);
		}
		}
		return true;
	}

	static bool HandleVipDebuffCommand(ChatHandler* handler, const char* /*args*/)
	{
		Player *chr = handler->GetSession()->GetPlayer();

		//Different Checks
		if (chr->IsInCombat() || chr->IsInFlight() || chr->GetMap()->IsBattlegroundOrArena() || chr->HasStealthAura() || chr->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || chr->isDead())
		{
			//handler->SendSysMessage(VIP_CANT_DO);
			handler->SetSentErrorMessage(true);
			return false;
		}

		if (!chr->GetSession()->IsPremium())
		{
			//handler->SendSysMessage(VIP_DONT_HAVE);
			handler->SetSentErrorMessage(true);
			return true;
		}

		handler->GetSession()->GetPlayer()->RemoveAurasDueToSpell(15007);
		handler->GetSession()->GetPlayer()->RemoveAurasDueToSpell(26013);

		return true;
	}

	static bool HandleVipMapCommand(ChatHandler* handler, const char* /*args*/)
	{
		Player *chr = handler->GetSession()->GetPlayer();

		//Different Checks
		if (chr->IsInCombat() || chr->IsInFlight() || chr->GetMap()->IsBattlegroundOrArena() || chr->HasStealthAura() || chr->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || chr->isDead())
		{
			//handler->SendSysMessage(VIP_CANT_DO);
			handler->SetSentErrorMessage(true);
			return false;
		}

		if (!chr->GetSession()->IsPremium() && !chr->IsGameMaster())
		{
			//handler->SendSysMessage(VIP_DONT_HAVE);
			handler->SetSentErrorMessage(true);
			return true;
		}

		handler->PSendSysMessage(LANG_YOU_SET_EXPLORE_ALL, handler->GetNameLink(chr).c_str());
		for (uint8 i = 0; i<PLAYER_EXPLORED_ZONES_SIZE; ++i)
		{
			handler->GetSession()->GetPlayer()->SetFlag(PLAYER_EXPLORED_ZONES_1 + i, 0xFFFFFFFF);
		}

		return true;
	}

	static bool HandleVipRepairCommand(ChatHandler* handler, const char* /*args*/)
	{
		Player *chr = handler->GetSession()->GetPlayer();

		//Different Checks
		if (chr->IsInCombat() || chr->IsInFlight() || chr->GetMap()->IsBattlegroundOrArena() || chr->HasStealthAura() || chr->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || chr->isDead())
		{
			//handler->SendSysMessage(VIP_CANT_DO);
			handler->SetSentErrorMessage(true);
			return false;
		}

		if (!chr->GetSession()->IsPremium() && !chr->IsGameMaster())
		{
			//handler->SendSysMessage(VIP_DONT_HAVE);
			handler->SetSentErrorMessage(true);
			return true;
		}

		// Repair items
		handler->GetSession()->GetPlayer()->DurabilityRepairAll(false, 0, false);

		handler->PSendSysMessage(LANG_YOUR_ITEMS_REPAIRED, handler->GetNameLink(handler->GetSession()->GetPlayer()).c_str());
		return true;
	}

	static bool HandleVipCapitalCommand(ChatHandler* handler, const char* /*args*/)
	{
		Player *chr = handler->GetSession()->GetPlayer();

		//Different Checks
		if (chr->IsInCombat() || chr->IsInFlight() || chr->GetMap()->IsBattlegroundOrArena() || chr->HasStealthAura() || chr->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || chr->isDead())
		{
			//handler->SendSysMessage(VIP_CANT_DO);
			handler->SetSentErrorMessage(true);
			return false;
		}

		if (!chr->GetSession()->IsPremium() && !chr->IsGameMaster())
		{
			//handler->SendSysMessage(VIP_DONT_HAVE);
			handler->SetSentErrorMessage(true);
			return true;
		}

		//chr->SetPhaseMask(2, true);

		if (chr->GetTeam() == HORDE)
			chr->CastSpell(chr, 3567, true);
		else
			chr->CastSpell(chr, 3561, true);

		return true;
	}
};
#define EMOTE_NO_VIP "Простите, вы не ВИП"
#define EMOTE_COOLDOWN "Can Not Do It This Time..."
#define EMOTE_NO_SICKENSS "You Dont Have Resurrection Sickness Aura!"
#define EMOTE_NO_DESERTER "У вас нету ауры дезертира!"
#define EMOTE_COMBAT "Вы в бою."
#define EMOTE_NO_ARENA_POINTS "You Have Enough Arena Points!"
#define EMOTE_ALREADY_ITEM "You Have Already Item!"

#define CONST_ARENA_RENAME 500
#define CONST_ARENA_CUSTOMIZE 500
#define CONST_ARENA_CHANGE_FACTION 1000
#define CONST_ARENA_CHANGE_RACE 1000

#define MSG_RENAME_COMPLETE "Make Logout and Enter the New Name of the Character. Do not forget After Name Change, to Bowl off And remove From the Client Cache Folder!"
#define MSG_CUSTOMIZE_COMPLETE2 "Make Logout and Change Appearance of the Character. Do not forget After Appearance Change, to Bowl off And remove From the Client Cache Folder!"
#define MSG_CHANGE_FACTION_COMPLETE2 "Make Logout and Change Fraction of the Character. Do not forget After Appearance Change, to Bowl off And remove From the Client Cache Folder!" 
#define MSG_CHANGE_RACE_COMPLETE "Make Logout and Change Race of the Character. Do not forget After Race of Appearance, to Bowl off And remove From the Client Cache Folder!"
#define MSG_MAX_SKILL "You Skills Up To Maximum!"
#define MSG_REMOVE_SICKNESS_COMPLETE "Resurrection Sickness Removed and Restore Full Health and Mana."
#define MSG_REMOVE_DESERTER_COMPLETE "Deserter Removed! You Can Again Will Return In The Battleground!"
#define MSG_RIDING_COMPLETE "Your Riding Skill Up To Maximum"
#define MSG_RESET_COOLDOWN "Your Cooldown Has Been Reset!"
#define MSG_CHARACTER_SAVE_TO_DB "You Character Has Been Saved!"
#define MSG_RESET_QUEST_STATUS_COMPLETE "Your Daily and Weeekly Quest Cooldown Has Been Reset!"

class npc_premium_master : public CreatureScript
{
public:
	npc_premium_master() : CreatureScript("npc_premium_master") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		if (!player->GetSession()->IsPremium())// && !player->IsGameMaster())
		{
			CloseGossipMenuFor(player);
			creature->Whisper(EMOTE_NO_VIP, LANG_UNIVERSAL, player);
			return true;
                }
		player->PlayerTalkClass->ClearMenus();
		AddGossipItemFor(player, 5, "Удалить слабость после воскрешения", GOSSIP_SENDER_MAIN, 1209);
		AddGossipItemFor(player, 5, "Удалить маску дезертира", GOSSIP_SENDER_MAIN, 1210);
		AddGossipItemFor(player, 5, "Излечиться", GOSSIP_SENDER_MAIN, 1202);
		//AddGossipItemFor(player, 5, "Сохранить персонажа", GOSSIP_SENDER_MAIN, 1213);
		AddGossipItemFor(player, 3, "Сбросить таланты", GOSSIP_SENDER_MAIN, 1207);
		AddGossipItemFor(player, 3, "Сбросить кулдауны", GOSSIP_SENDER_MAIN, 1212);
		//AddGossipItemFor(player, 3, "Сбросить кулдауны квестов", GOSSIP_SENDER_MAIN, 1215);
		AddGossipItemFor(player, 3, "Прокачать скилы", GOSSIP_SENDER_MAIN, 1204);
		AddGossipItemFor(player, 3, "Прокачать верховую езду", GOSSIP_SENDER_MAIN, 1211);
		AddGossipItemFor(player, 7, "Получить сумки", GOSSIP_SENDER_MAIN, 1216);
		/*AddGossipItemFor(player, 9, "Mounts Rare ->", GOSSIP_SENDER_MAIN, 1208);
		AddGossipItemFor(player, 9, "Mounts Elite ->", GOSSIP_SENDER_MAIN, 1217);
		AddGossipItemFor(player, 9, "Mounts Proto ->", GOSSIP_SENDER_MAIN, 1218);
		AddGossipItemFor(player, 6, "Morph ->", GOSSIP_SENDER_MAIN, 1203);*/
		AddGossipItemFor(player, 6, "Бафы ->", GOSSIP_SENDER_MAIN, 1206);
		/**/AddGossipItemFor(player, 8, "Customize ->", GOSSIP_SENDER_MAIN, 1205);
		AddGossipItemFor(player, 8, "Movie ->", GOSSIP_SENDER_MAIN, 1214);
		SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		return true;
	}
	bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 action)
	{
		if (!player->getAttackers().empty())
		{
			creature->Whisper(EMOTE_COMBAT, LANG_UNIVERSAL, player);
			CloseGossipMenuFor(player);
			return false;
		}

		player->PlayerTalkClass->ClearMenus();

		switch (action)
		{

		case 1202: // Heal
			if (player->HasAura(45523))
			{
				CloseGossipMenuFor(player);
				creature->Whisper(EMOTE_COOLDOWN, LANG_UNIVERSAL, player);
			}
			else
			{
				CloseGossipMenuFor(player);
				player->CastSpell(player, 25840, true);
				player->CastSpell(player, 45523, true);
			}
			break;
		case 1207: // Reset talents
			CloseGossipMenuFor(player);
			player->SendTalentWipeConfirm(creature->GetGUID());
			break;
		case 1203: // Morphs
			player->PlayerTalkClass->ClearMenus();
			AddGossipItemFor(player, 5, "Star Craft Murlock", GOSSIP_SENDER_MAIN, 499);
			AddGossipItemFor(player, 5, "Ambrose Boltspark", GOSSIP_SENDER_MAIN, 500);
			AddGossipItemFor(player, 5, "Jonathan The Revelator", GOSSIP_SENDER_MAIN, 501);
			AddGossipItemFor(player, 5, "Magi", GOSSIP_SENDER_MAIN, 502);
			AddGossipItemFor(player, 5, "Worgen", GOSSIP_SENDER_MAIN, 503);
			AddGossipItemFor(player, 5, "Pandaren Monk", GOSSIP_SENDER_MAIN, 504);
			AddGossipItemFor(player, 5, "Crok Scourgebane", GOSSIP_SENDER_MAIN, 505);
			AddGossipItemFor(player, 5, "Iron Mender", GOSSIP_SENDER_MAIN, 506);
			AddGossipItemFor(player, 5, "Druid", GOSSIP_SENDER_MAIN, 507);
			AddGossipItemFor(player, 5, "Priest", GOSSIP_SENDER_MAIN, 508);
			AddGossipItemFor(player, 5, "Paladin", GOSSIP_SENDER_MAIN, 509);
			AddGossipItemFor(player, 5, "Rogue", GOSSIP_SENDER_MAIN, 510);
			AddGossipItemFor(player, 5, "Death Knight", GOSSIP_SENDER_MAIN, 511);
			AddGossipItemFor(player, 5, "Warlock", GOSSIP_SENDER_MAIN, 512);
			AddGossipItemFor(player, 5, "Warrior", GOSSIP_SENDER_MAIN, 513);
			AddGossipItemFor(player, 5, "Mage", GOSSIP_SENDER_MAIN, 514);
			AddGossipItemFor(player, 5, "Shaman", GOSSIP_SENDER_MAIN, 515);
			AddGossipItemFor(player, 5, "Hunter", GOSSIP_SENDER_MAIN, 516);
			AddGossipItemFor(player, 5, "Draenei Girl White", GOSSIP_SENDER_MAIN, 517);
			AddGossipItemFor(player, 5, "Draenei Girl Black", GOSSIP_SENDER_MAIN, 518);
			AddGossipItemFor(player, 0, "Demorph", GOSSIP_SENDER_MAIN, 519);
			SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
			break;
		case 499:
			CloseGossipMenuFor(player); // Starcraft Murlock
			player->SetDisplayId(29348);
			break;
		case 500:
			CloseGossipMenuFor(player); // Ambrose Boltspark
			player->SetDisplayId(28586);
			break;
		case 501:
			CloseGossipMenuFor(player); // Jonathan The Revelator
			player->SetDisplayId(15867);
			break;
		case 502:
			CloseGossipMenuFor(player); // Shattered Sun Magi
			player->SetDisplayId(22959);
			break;
		case 503:
			CloseGossipMenuFor(player); // Worgen
			player->SetDisplayId(657);
			break;
		case 504:
			CloseGossipMenuFor(player); // Pandaren Monk
			player->SetDisplayId(30414);
			break;
		case 505:
			CloseGossipMenuFor(player); // Crok Scourgebane
			player->SetDisplayId(30911);
			break;
		case 506:
			CloseGossipMenuFor(player); // Iron Mender
			player->SetDisplayId(26239);
			break;
		case 507:
			CloseGossipMenuFor(player); // Druid
			player->SetDisplayId(30472);
			break;
		case 508:
			CloseGossipMenuFor(player); // Priest
			player->SetDisplayId(21419);
			break;
		case 509:
			CloseGossipMenuFor(player); // Paladin
			player->SetDisplayId(29774);
			break;
		case 510:
			CloseGossipMenuFor(player); // Rogue
			player->SetDisplayId(30485);
			break;
		case 511:
			CloseGossipMenuFor(player); // Death Knight
			player->SetDisplayId(27153);
			break;
		case 512:
			CloseGossipMenuFor(player); // Warlock
			player->SetDisplayId(30487);
			break;
		case 513:
			CloseGossipMenuFor(player); // Warrior
			player->SetDisplayId(30474);
			break;
		case 514:
			CloseGossipMenuFor(player); // Mage
			player->SetDisplayId(30477);
			break;
		case 515:
			CloseGossipMenuFor(player); // Shaman 
			player->SetDisplayId(29768);
			break;
		case 516:
			CloseGossipMenuFor(player); // Hunter
			player->SetDisplayId(21379);
			break;
		case 517:
			CloseGossipMenuFor(player); // Coliseum Champion
			player->SetDisplayId(30634);
			break;
		case 518:
			CloseGossipMenuFor(player); // Coliseum Champion
			player->SetDisplayId(30771);
			break;
		case 519: // Demorph Player (remove all morphs)
			CloseGossipMenuFor(player);
			player->DeMorph();
			break;
		case 1206: // Buffs
			player->PlayerTalkClass->ClearMenus();
			AddGossipItemFor(player, 5, "Power Word Fortitude", GOSSIP_SENDER_MAIN, 4000);
			AddGossipItemFor(player, 5, "Prayer of Spirit", GOSSIP_SENDER_MAIN, 4001);
			AddGossipItemFor(player, 5, "Shadow Protection", GOSSIP_SENDER_MAIN, 4002);
			AddGossipItemFor(player, 5, "Greater Blessing of Kings", GOSSIP_SENDER_MAIN, 4003);
			/*AddGossipItemFor(player, 5, "Greater Bleesing of Might", GOSSIP_SENDER_MAIN,4004);
			AddGossipItemFor(player, 5, "Greater Blessing of Wisdom", GOSSIP_SENDER_MAIN, 4005);
			AddGossipItemFor(player, 5, "Greater Blessing of Sanctuary", GOSSIP_SENDER_MAIN, 4006);*/
			AddGossipItemFor(player, 5, "Arane Intellect", GOSSIP_SENDER_MAIN, 4007);
			AddGossipItemFor(player, 5, "Dampen Magic", GOSSIP_SENDER_MAIN, 4008);
			AddGossipItemFor(player, 5, "Amplify Magic", GOSSIP_SENDER_MAIN, 4009);
			// AddGossipItemFor(player, 5, "Gift of the Wild", GOSSIP_SENDER_MAIN, 4010);
			AddGossipItemFor(player, 5, "Thorns", GOSSIP_SENDER_MAIN, 4011);
			AddGossipItemFor(player, 5, "Brilliance Intellect", GOSSIP_SENDER_MAIN, 4012);
			SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
			break;
		case 4000: // Power Word Fortitude
			CloseGossipMenuFor(player);
			player->CastSpell(player, 48162, true);
			break;
		case 4001: // Prayer of Spirit
			CloseGossipMenuFor(player);
			player->CastSpell(player, 48074, true);
			break;
		case 4002: // Shadow Protection
			CloseGossipMenuFor(player);
			player->CastSpell(player, 48170, true);
			break;
		case 4003: // Greater Blessing of Kings
			CloseGossipMenuFor(player);
			player->CastSpell(player, 43223, true);
			break;
		case 4004: // Greater Bleesing of Might
			CloseGossipMenuFor(player);
			player->CastSpell(player, 48934, true);
			break;
		case 4005: // Greater Blessing of Wisdom
			CloseGossipMenuFor(player);
			player->CastSpell(player, 48938, true);
			break;
		case 4006: // Greater Blessing of Sanctuary
			CloseGossipMenuFor(player);
			player->CastSpell(player, 25899, true);
			break;
		case 4007: // Arane Intellect
			CloseGossipMenuFor(player);
			player->CastSpell(player, 36880, true);
			break;
		case 4008: // Dampen Magic
			CloseGossipMenuFor(player);
			player->CastSpell(player, 43015, true);
			break;
		case 4009: // Amplify Magic
			CloseGossipMenuFor(player);
			player->CastSpell(player, 43017, true);
			break;
		case 4010: // Gift of the Wild
			CloseGossipMenuFor(player);
			player->CastSpell(player, 69381, true);
			break;
		case 4011: // Thorns
			CloseGossipMenuFor(player);
			player->CastSpell(player, 467, true);
			break;
		case 4012: // Brilliance Intellect
			CloseGossipMenuFor(player);
			player->CastSpell(player, 69994, true);
			break;
		case 1205:
			player->PlayerTalkClass->ClearMenus();
			AddGossipItemFor(player, 0, "Rename [500 Arena Points]", GOSSIP_SENDER_MAIN, 2000);
			AddGossipItemFor(player, 0, "Customize [500 Arena Points]", GOSSIP_SENDER_MAIN, 2001);
			AddGossipItemFor(player, 0, "Change Faction [1000 Arena Points]", GOSSIP_SENDER_MAIN, 2002);
			AddGossipItemFor(player, 0, "Change Race [1000 Arena Points]", GOSSIP_SENDER_MAIN, 2003);
			SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
			break;
		case 2000: // Rename
			if (player->GetArenaPoints() < CONST_ARENA_RENAME)
			{
				creature->Whisper(EMOTE_NO_ARENA_POINTS, LANG_UNIVERSAL, player);
				CloseGossipMenuFor(player);
			}
			else
			{
				CloseGossipMenuFor(player);
				player->SetAtLoginFlag(AT_LOGIN_RENAME);
				player->ModifyArenaPoints(-CONST_ARENA_RENAME);
				creature->Whisper(MSG_RENAME_COMPLETE, LANG_UNIVERSAL, player);
			}
			break;
		case 2001: // Customize
			if (player->GetArenaPoints() < CONST_ARENA_CUSTOMIZE)
			{
				creature->Whisper(EMOTE_NO_ARENA_POINTS, LANG_UNIVERSAL, player);
				CloseGossipMenuFor(player);
			}
			else
			{
				CloseGossipMenuFor(player);
				player->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
				player->ModifyArenaPoints(-CONST_ARENA_CUSTOMIZE);
				creature->Whisper(MSG_CUSTOMIZE_COMPLETE2, LANG_UNIVERSAL, player);
			}
			break;
		case 2002: // Change Faction
			if (player->GetArenaPoints() < CONST_ARENA_CHANGE_FACTION)
			{
				creature->Whisper(EMOTE_NO_ARENA_POINTS, LANG_UNIVERSAL, player);
				CloseGossipMenuFor(player);
			}
			else
			{
				CloseGossipMenuFor(player);
				player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
				player->ModifyArenaPoints(-CONST_ARENA_CHANGE_FACTION);
				creature->Whisper(MSG_CHANGE_FACTION_COMPLETE2, LANG_UNIVERSAL, player);
			}
			break;
		case 2003: // Change Race
			if (player->GetArenaPoints() < CONST_ARENA_CHANGE_RACE)
			{
				creature->Whisper(EMOTE_NO_ARENA_POINTS, LANG_UNIVERSAL, player);
				CloseGossipMenuFor(player);
			}
			else
			{
				CloseGossipMenuFor(player);
				player->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
				player->ModifyArenaPoints(-CONST_ARENA_CHANGE_RACE);
				creature->Whisper(MSG_CHANGE_RACE_COMPLETE, LANG_UNIVERSAL, player);
			}
			break;
		case 1204: // Max Skills
			CloseGossipMenuFor(player);
			player->UpdateSkillsToMaxSkillsForLevel();
			creature->Whisper(MSG_MAX_SKILL, LANG_UNIVERSAL, player);
			break;
		case 1208: // Mounts Ground
			player->PlayerTalkClass->ClearMenus();
			AddGossipItemFor(player, 5, "Black Qiraji Resonating Crystal", GOSSIP_SENDER_MAIN, 100);
			AddGossipItemFor(player, 5, "Amani War Bear", GOSSIP_SENDER_MAIN, 101);
			AddGossipItemFor(player, 5, "Big Battle Bear", GOSSIP_SENDER_MAIN, 102);
			AddGossipItemFor(player, 5, "Deathcharger's Reins", GOSSIP_SENDER_MAIN, 103);
			AddGossipItemFor(player, 5, "Fiery Warhorse's Reins", GOSSIP_SENDER_MAIN, 104);
			AddGossipItemFor(player, 5, "Swift Burgundy Wolf", GOSSIP_SENDER_MAIN, 105);
			AddGossipItemFor(player, 5, "Great Brewfest Kodo", GOSSIP_SENDER_MAIN, 106);
			AddGossipItemFor(player, 5, "Horn of the Frostwolf Howler", GOSSIP_SENDER_MAIN, 107);
			AddGossipItemFor(player, 5, "Magic Rooster Egg", GOSSIP_SENDER_MAIN, 108);
			AddGossipItemFor(player, 5, "Reins of the White Polar Bear", GOSSIP_SENDER_MAIN, 109);
			AddGossipItemFor(player, 5, "Reins of the Brown Polar Bear", GOSSIP_SENDER_MAIN, 110);
			AddGossipItemFor(player, 5, "Reins of the Crimson Deathcharger", GOSSIP_SENDER_MAIN, 111);
			AddGossipItemFor(player, 5, "Reins of the Raven Lord", GOSSIP_SENDER_MAIN, 112);
			AddGossipItemFor(player, 5, "Reins of the Swift Spectral Tiger", GOSSIP_SENDER_MAIN, 113);
			AddGossipItemFor(player, 5, "Sea Turtle", GOSSIP_SENDER_MAIN, 114);
			AddGossipItemFor(player, 5, "Swift Razzashi Raptor", GOSSIP_SENDER_MAIN, 115);
			AddGossipItemFor(player, 5, "Swift White Hawkstrider", GOSSIP_SENDER_MAIN, 116);
			AddGossipItemFor(player, 5, "Swift Zhevra", GOSSIP_SENDER_MAIN, 117);
			AddGossipItemFor(player, 5, "Swift Zulian Tiger", GOSSIP_SENDER_MAIN, 118);
			AddGossipItemFor(player, 5, "The Horseman's Reins", GOSSIP_SENDER_MAIN, 119);
			AddGossipItemFor(player, 5, "Wooly White Rhino", GOSSIP_SENDER_MAIN, 120);
			AddGossipItemFor(player, 5, "Four Qiraji Resonating Crystal", GOSSIP_SENDER_MAIN, 121);
			SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
			break;
		case 100: // Black Qiraji Resonating Crystal
			CloseGossipMenuFor(player);
			player->AddItem(21176, 1);
			break;
		case 101: // Amani War Bear
			CloseGossipMenuFor(player);
			player->AddItem(33809, 1);
			break;
		case 102: // Big Battle Bear
			CloseGossipMenuFor(player);
			player->AddItem(38576, 1);
			break;
		case 103: // Deathcharger's Reins
			CloseGossipMenuFor(player);
			player->AddItem(13335, 1);
			break;
		case 104: // Fiery Warhorse's Reins
			CloseGossipMenuFor(player);
			player->AddItem(30480, 1);
			break;
		case 105: // Swift Burgundy Wolf
			CloseGossipMenuFor(player);
			player->CastSpell(player, 65646, true); // Cast Metod Because Item (Faction Flag)
			break;
		case 106: // Great Brewfest Kodo
			CloseGossipMenuFor(player);
			player->AddItem(37828, 1);
			break;
		case 107: // Horn of the Frostwolf Howler
			CloseGossipMenuFor(player);
			player->CastSpell(player, 23509, true); // Cast Metod Because Item (Faction Flag)
			break;
		case 108: // Magic Rooster Egg
			CloseGossipMenuFor(player);
			player->AddItem(46778, 1);
			break;
		case 109: // Reins of the White Polar Bear
			CloseGossipMenuFor(player);
			player->AddItem(43962, 1);
			break;
		case 110: // Reins of the Brown Polar Bear
			CloseGossipMenuFor(player);
			player->AddItem(43963, 1);
			break;
		case 111: // Reins of the Crimson Deathcharger
			CloseGossipMenuFor(player);
			player->AddItem(52200, 1);
			break;
		case 112: // Reins of the Raven Lord
			CloseGossipMenuFor(player);
			player->AddItem(32768, 1);
			break;
		case 113: // Reins of the Swift Spectral Tiger
			CloseGossipMenuFor(player);
			player->AddItem(33225, 1);
			break;
		case 114: // Sea Turtle
			CloseGossipMenuFor(player);
			player->AddItem(46109, 1);
			break;
		case 115: // Swift Razzashi Raptor
			CloseGossipMenuFor(player);
			player->AddItem(19872, 1);
			break;
		case 116: // Swift White Hawkstrider
			CloseGossipMenuFor(player);
			player->AddItem(35513, 1);
			break;
		case 117: // Swift Zhevra
			CloseGossipMenuFor(player);
			player->AddItem(37719, 1);
			break;
		case 118: // Swift Zulian Tiger
			CloseGossipMenuFor(player);
			player->AddItem(19902, 1);
			break;
		case 119: // The Horseman's Reins
			CloseGossipMenuFor(player);
			player->AddItem(37012, 1);
			break;
		case 120: // Wooly White Rhino
			CloseGossipMenuFor(player);
			player->AddItem(54068, 1);
			break;
		case 121: // Four Qiraji Resonating Crystal
			CloseGossipMenuFor(player);
			player->AddItem(21218, 1); // Blue Qiraji Resonating Crystal
			player->AddItem(21323, 1); // Green Qiraji Resonating Crystal
			player->AddItem(21321, 1); // Red Qiraji Resonating Crystal
			player->AddItem(21324, 1); // Yellow Qiraji Resonating Crystal
			break;
		case 1217: // Mounts Fly
			player->PlayerTalkClass->ClearMenus();
			AddGossipItemFor(player, 5, "Argent Hippogryph", GOSSIP_SENDER_MAIN, 122);
			AddGossipItemFor(player, 5, "Ashes of Alar", GOSSIP_SENDER_MAIN, 123);
			AddGossipItemFor(player, 5, "Big Love Rocket", GOSSIP_SENDER_MAIN, 124);
			AddGossipItemFor(player, 5, "Blazing Hippogryph", GOSSIP_SENDER_MAIN, 125);
			AddGossipItemFor(player, 5, "Swift Nether Drake", GOSSIP_SENDER_MAIN, 126);
			AddGossipItemFor(player, 5, "Vengeful Nether Drake", GOSSIP_SENDER_MAIN, 127);
			AddGossipItemFor(player, 5, "Merciless Nether Drake", GOSSIP_SENDER_MAIN, 128);
			AddGossipItemFor(player, 5, "Brutal Nether Drake", GOSSIP_SENDER_MAIN, 129);
			AddGossipItemFor(player, 5, "Celestial Steed", GOSSIP_SENDER_MAIN, 130);
			AddGossipItemFor(player, 5, "Deadly Gladiators Frost Wyrm", GOSSIP_SENDER_MAIN, 131);
			AddGossipItemFor(player, 5, "Furious Gladiators Frost Wyrm", GOSSIP_SENDER_MAIN, 132);
			AddGossipItemFor(player, 5, "Relentless Gladiators Frost Wyrm", GOSSIP_SENDER_MAIN, 133);
			AddGossipItemFor(player, 5, "Wrathful Gladiators Frost Wyrm", GOSSIP_SENDER_MAIN, 134);
			AddGossipItemFor(player, 5, "Invincibles Reins", GOSSIP_SENDER_MAIN, 135);
			AddGossipItemFor(player, 5, "Mimirons Head", GOSSIP_SENDER_MAIN, 136);
			AddGossipItemFor(player, 5, "Reins of the Albino Drake", GOSSIP_SENDER_MAIN, 137);
			AddGossipItemFor(player, 5, "Reins of the Azure Drake", GOSSIP_SENDER_MAIN, 138);
			AddGossipItemFor(player, 5, "Reins of the Black Drake", GOSSIP_SENDER_MAIN, 139);
			AddGossipItemFor(player, 5, "Reins of the Blue Drake", GOSSIP_SENDER_MAIN, 140);
			AddGossipItemFor(player, 5, "Reins of the Bronze Drake", GOSSIP_SENDER_MAIN, 141);
			AddGossipItemFor(player, 5, "Reins of the Twilight Drake", GOSSIP_SENDER_MAIN, 152);
			AddGossipItemFor(player, 5, "Reins of the Onyxian Drake", GOSSIP_SENDER_MAIN, 142);
			AddGossipItemFor(player, 5, "X-51 Nether-Rocket X-TREME", GOSSIP_SENDER_MAIN, 143);
			AddGossipItemFor(player, 5, "X-53 Touring Rocket", GOSSIP_SENDER_MAIN, 144);
			AddGossipItemFor(player, 5, "Frozen Frost Wyrm Heart", GOSSIP_SENDER_MAIN, 145);
			SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
			break;
		case 122: // Argent Hippogryph
			CloseGossipMenuFor(player);
			player->AddItem(45725, 1);
			break;
		case 123: // Ashes of Alar
			CloseGossipMenuFor(player);
			player->AddItem(32458, 1);
			break;
		case 124: // Big Love Rocket
			CloseGossipMenuFor(player);
			player->AddItem(50520, 1);
			break;
		case 125: // Blazing Hippogryph
			CloseGossipMenuFor(player);
			player->AddItem(54069, 1);
			break;
		case 126: // Swift Nether Drake
			CloseGossipMenuFor(player);
			player->AddItem(30609, 1);
			break;
		case 127: // Vengeful Nether Drake
			CloseGossipMenuFor(player);
			player->AddItem(37676, 1);
			break;
		case 128: // Merciless Nether Drake
			CloseGossipMenuFor(player);
			player->AddItem(34092, 1);
			break;
		case 129: // Brutal Nether Drake
			CloseGossipMenuFor(player);
			player->AddItem(43516, 1);
			break;
		case 130: // Celestial Steed
			CloseGossipMenuFor(player);
			player->AddItem(54811, 1);
			break;
		case 131: // Deadly Gladiator's Frost Wyrm
			CloseGossipMenuFor(player);
			player->AddItem(46708, 1);
			break;
		case 132: // Furious Gladiator's Frost Wyrm
			CloseGossipMenuFor(player);
			player->AddItem(46171, 1);
			break;
		case 133: // Relentless Gladiator's Frost Wyrm
			CloseGossipMenuFor(player);
			player->AddItem(47840, 1);
			break;
		case 134: // Wrathful Gladiator's Frost Wyrm
			CloseGossipMenuFor(player);
			player->AddItem(50435, 1);
			break;
		case 135: // Invincibles Reins
			CloseGossipMenuFor(player);
			player->AddItem(50818, 1);
			break;
		case 136: // Mimiron's Head
			CloseGossipMenuFor(player);
			player->AddItem(45693, 1);
			break;
		case 137: // Reins of the Albino Drake
			CloseGossipMenuFor(player);
			player->AddItem(44178, 1);
			break;
		case 138: // Reins of the Azure Drake
			CloseGossipMenuFor(player);
			player->AddItem(43952, 1);
			break;
		case 139: // Reins of the Black Drake
			CloseGossipMenuFor(player);
			player->AddItem(43986, 1);
			break;
		case 140: // Reins of the Blue Drake
			CloseGossipMenuFor(player);
			player->AddItem(43953, 1);
			break;
		case 141: // Reins of the Bronze Drake
			CloseGossipMenuFor(player);
			player->AddItem(43951, 1);
			break;
		case 142: // Reins of the Onyxian Drake
			CloseGossipMenuFor(player);
			player->AddItem(49636, 1);
			break;
		case 152: // Reins of the Twilight Drake
			CloseGossipMenuFor(player);
			player->AddItem(43954, 1);
			break;
		case 143: // X-51 Nether-Rocket X-TREME
			CloseGossipMenuFor(player);
			player->AddItem(35226, 1);
			break;
		case 144: // X-53 Touring Rocket
			CloseGossipMenuFor(player);
			player->AddItem(54860, 1);
			break;
		case 145: // Frozen Frost Wyrm Heart
			CloseGossipMenuFor(player);   // 33182 Swift Flying Broom // // 33184 Swift Magic Broom // 37011 Magic Broom // 33183 ld Magic Broom
			player->AddItem(38690, 1);
			break;
		case 1218:
			player->PlayerTalkClass->ClearMenus();
			AddGossipItemFor(player, 5, "Reins of the Black Proto-Drake", GOSSIP_SENDER_MAIN, 160);
			AddGossipItemFor(player, 5, "Reins of the Blue Proto-Drake", GOSSIP_SENDER_MAIN, 161);
			AddGossipItemFor(player, 5, "Reins of the Green Proto-Drake", GOSSIP_SENDER_MAIN, 162);
			AddGossipItemFor(player, 5, "Reins of the Ironbound Proto-Drake", GOSSIP_SENDER_MAIN, 163);
			AddGossipItemFor(player, 5, "Reins of the Plagued Proto-Drake", GOSSIP_SENDER_MAIN, 164);
			AddGossipItemFor(player, 5, "Reins of the Red Proto-Drake", GOSSIP_SENDER_MAIN, 165);
			AddGossipItemFor(player, 5, "Reins of the Rusted Proto-Drake", GOSSIP_SENDER_MAIN, 166);
			AddGossipItemFor(player, 5, "Reins of the Time-Lost Proto-Drake", GOSSIP_SENDER_MAIN, 167);
			AddGossipItemFor(player, 5, "Reins of the Violet Proto-Drake", GOSSIP_SENDER_MAIN, 168);
			SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
			break;
		case 160: // Reins of the Black Proto-Drake
			CloseGossipMenuFor(player);
			player->AddItem(44164, 1);
			break;
		case 161: // Reins of the Blue Proto-Drake
			CloseGossipMenuFor(player);
			player->AddItem(44151, 1);
			break;
		case 162: // Reins of the Green Proto-Drake
			CloseGossipMenuFor(player);
			player->AddItem(44707, 1);
			break;
		case 163: // Reins of the Ironbound Proto-Drake
			CloseGossipMenuFor(player);
			player->AddItem(45801, 1);
			break;
		case 164: // Reins of the Plagued Proto-Drake
			CloseGossipMenuFor(player);
			player->AddItem(44175, 1);
			break;
		case 165: // Reins of the Red Proto-Drake
			CloseGossipMenuFor(player);
			player->AddItem(44160, 1);
			break;
		case 166: // Reins of the Rusted Proto-Drake
			CloseGossipMenuFor(player);
			player->AddItem(45802, 1);
			break;
		case 167: // Reins of the Time-Lost Proto-Drake
			CloseGossipMenuFor(player);
			player->AddItem(44168, 1);
			break;
		case 168: // Reins of the Violet Proto-Drake
			CloseGossipMenuFor(player);
			player->AddItem(44177, 1);
			break;
		case 1209: // Remove Resurrection Sickness Aura
			if (!player->HasAura(15007))
			{
				CloseGossipMenuFor(player);
				creature->Whisper(EMOTE_NO_SICKENSS, LANG_UNIVERSAL, player);
			}
			else
			{
				CloseGossipMenuFor(player);
				player->RemoveAurasDueToSpell(15007);
				player->SetHealth(player->GetMaxHealth()); //Restore Health
				player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA)); // Restore Mana
				creature->Whisper(MSG_REMOVE_SICKNESS_COMPLETE, LANG_UNIVERSAL, player);
			}
			break;
		case 1210: // Remove Deserter Aura
			if (!player->HasAura(26013))
			{
				CloseGossipMenuFor(player);
				creature->Whisper(EMOTE_NO_DESERTER, LANG_UNIVERSAL, player);
			}
			else
			{
				CloseGossipMenuFor(player);
				player->RemoveAurasDueToSpell(26013);
				creature->Whisper(MSG_REMOVE_DESERTER_COMPLETE, LANG_UNIVERSAL, player);
			}
			break;
		case 1211: // Riding Skills
			CloseGossipMenuFor(player);
			player->CastSpell(player, 33389, true); // Apprentice Riding
			player->CastSpell(player, 33392, true); // Journeyman Riding
			player->CastSpell(player, 34092, true); // Expert Riding
			player->CastSpell(player, 34093, true); // Artisan Riding
			player->CastSpell(player, 54197, true); // Allows you to ride flying mounts in Northrend.
			creature->Whisper(MSG_RIDING_COMPLETE, LANG_UNIVERSAL, player);
			break;
		case 1212: // Reset Cooldown
			CloseGossipMenuFor(player);
			player->RemoveArenaSpellCooldowns();
			creature->Whisper(MSG_RESET_COOLDOWN, LANG_UNIVERSAL, player);
			break;
		case 1213: // Save Character Online
			CloseGossipMenuFor(player);
			player->SaveToDB();
			creature->Whisper(MSG_CHARACTER_SAVE_TO_DB, LANG_UNIVERSAL, player);
			break;
		case 1214: // Movie Menu
			player->PlayerTalkClass->ClearMenus();
			AddGossipItemFor(player, 5, "World of Warcraft", GOSSIP_SENDER_MAIN, 250);
			AddGossipItemFor(player, 5, "The Wrath Gate", GOSSIP_SENDER_MAIN, 251);
			AddGossipItemFor(player, 5, "Fall of The Lich King", GOSSIP_SENDER_MAIN, 252);
			SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
			break;
		case 250: // Classic
			CloseGossipMenuFor(player);
			player->SendMovieStart(2);
			break;
		case 251: // The Wrath Gate
			CloseGossipMenuFor(player);
			player->SendMovieStart(14);
			break;
		case 252: // Fall of The Lich King
			CloseGossipMenuFor(player);
			player->SendMovieStart(16);
			break;
		case 1215: // Reset Daily and Weekly Quest Cooldown
			CloseGossipMenuFor(player);
			player->ResetDailyQuestStatus();
			player->ResetWeeklyQuestStatus();
			creature->Whisper(MSG_RESET_QUEST_STATUS_COMPLETE, LANG_UNIVERSAL, player);
			break;
		case 1216: // Bag
			if (player->HasItemCount(38082, 4))
			{
				CloseGossipMenuFor(player);
				creature->Whisper(EMOTE_ALREADY_ITEM, LANG_UNIVERSAL, player);
			}
			else
			{
				CloseGossipMenuFor(player);
				player->AddItem(38082, 4);
			}
			break;
		}
		return true;
	}
};

	
void AddSC_premium_commandscript()
{
    new premium_commandscript();
	new npc_premium_master;
}
