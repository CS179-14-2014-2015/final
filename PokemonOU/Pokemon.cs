using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace PokemonOU
{
    class Pokemon
    {
        private int type1, type2, hp, attack, defense, spatk, spdef, speed;
        private string name;

        public string Name
        {
            get { return name; }
            set { name = value; }
        }

        public int Speed
        {
            get { return speed; }
            set { speed = value; }
        }

        public int Spdef
        {
            get { return spdef; }
            set { spdef = value; }
        }

        public int Spatk
        {
            get { return spatk; }
            set { spatk = value; }
        }

        public int Defense
        {
            get { return defense; }
            set { defense = value; }
        }

        public int Attack
        {
            get { return attack; }
            set { attack = value; }
        }

        public int Hp
        {
            get { return hp; }
            set { hp = value; }
        }

        public int Type2
        {
            get { return type2; }
            set { type2 = value; }
        }

        public int Type1
        {
            get { return type1; }
            set { type1 = value; }
        } //properties

        //TYPE LEGEND:
        //normal = 1
        //fighting = 2
        //flying = 3
        //poison = 4
        //ground = 5
        //rock = 6
        //bug = 7
        //ghost = 8
        //steel = 9
        //fire = 10
        //water = 11
        //grass = 12
        //electric = 13
        //psychic = 14
        //ice = 15
        //dragon = 16
        //dark = 17
        //fairy = 18

        public Pokemon(string entryname, int t1, int t2, int base_hp, int base_atk, int base_def, int base_spatk, int base_spdef, int base_spd)
        {
            this.name = entryname;
            this.type1 = t1;
            this.type2 = t2;
            this.hp = (((31 + (2 * base_hp) + 100) * 100) / 100) + 10;
            this.attack = (((31 + (2 * base_atk)) * 100) / 100) + 5;
            this.defense = (((31 + (2 * base_def)) * 100) / 100) + 5;
            this.spatk = (((31 + (2 * base_spatk)) * 100) / 100) + 5;
            this.spdef = (((31 + (2 * base_spdef)) * 100) / 100) + 5;
            this.speed = (((31 + (2 * base_spd)) * 100) / 100) + 5;
        }

        public override string ToString()
        {
            return name;
        }
    }
}
