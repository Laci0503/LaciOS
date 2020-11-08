#include "../headers/memory.h"

uint8 mem_get_state(uint8 piece){
    return (memory_state[piece/8] & (0x1 << (piece % 8))) != 0;
}
void mem_set_state(uint8 piece, uint8 state){
    if(state){
        memory_state[piece/8] |= 0x1 << (piece % 8);
    }else{
        memory_state[piece/8] &= ~(0x1 << (piece % 8));
    }
}

void load_memory_map(){
    //print("Loading memory map");
    //nexLine();
    uint64* mmap_address = (uint64*) 0x500;
    memroy_map_region_base = (uint64*) 0x6E0;
    memory_map_region_length = (uint64*) 0x6E0+8*10;
    memory_map_region_type = (uint8*)0x6E0+8*10*2;

    uint8 index=0;
    uint64 ba=0;

    for(uint8 i = 0;i<10;i++){
        ba=*mmap_address;
        mmap_address++;
        if(*mmap_address!=0){
            memroy_map_region_base[index]=ba;
            memory_map_region_length[index]=*mmap_address;
            mmap_address++;
            memory_map_region_type[index]=(uint8)(*mmap_address & 0xff);
            mmap_address++;
            index++;
        }else{
            mmap_address+=2;
        }
    }
    memory_map_region_count=index;
}
uint32 avaible_ram;

void free(uint8* address, uint32 length){
    /*print("Freed memory from 0x");
    printhex(address);
    print(" to 0x");
    printhex(address+length-1);
    print(", ");
    printint(length);
    print(" bytes");
    nextLine();*/

    address-=MEM_BASE;

    #if (MEM_DEBUG==1)
        print("Free: address: 0x");
        printhex(address+MEM_BASE);
        print(", length: ");
        printint(length);
        print(", end: 0x");
        printhex(address+length+MEM_BASE);
        print(" ");
    #endif

    uint8 pos=0;
    uint8 can_free=0;

    for(uint8 i=0;i<memory_piece_count;i++){
        if(memory_pieces[i]<=(uint32)address && (i==memory_piece_count-1 ? heap_size : memory_pieces[i+1])>(uint32)address){
            pos=i;
            can_free=1;
            break;
        }
    }
    if (can_free){
        #if (MEM_DEBUG==1)
        if((uint32)address+length>(pos==memory_piece_count-1 ? heap_size : memory_pieces[pos+1])){
            print("Memory error: Free; Length is bigger than the memory segment length, original length: ");
            printint(length);
            length=(pos==memory_piece_count-1 ? heap_size : memory_pieces[pos+1])-(uint32)address;
            print(", new length: ");
            printint(length);
            nextLine();
        }
        #endif

        uint8 eleje_annyi = memory_pieces[pos] == (uint32)address;
        uint8 vege_annyi= (pos==memory_piece_count-1 ? heap_size : memory_pieces[pos+1])==(uint32)address+length;

        if(eleje_annyi&&vege_annyi){
            #if (MEM_DEBUG==1)
                print("e&v");
            #endif
            if(pos!=0){
                for(uint8 i=pos+2;i<memory_piece_count;i++){
                    memory_pieces[i-2]=memory_pieces[i];
                }
                memory_piece_count-=2;
            }else{
                for(uint8 i=pos+1;i<memory_piece_count;i++){
                    memory_pieces[i-1]=memory_pieces[i];
                    mem_set_state(i-1,mem_get_state(i));
                }
            }
        }else if(eleje_annyi && !vege_annyi){
            #if (MEM_DEBUG==1)
                print("e&!v");
            #endif
            if(pos!=0){
                memory_pieces[pos]=(uint32)address+length;
            }else{
                mem_set_state(0,MEM_STATE_FREE);
                for(uint8 i=memory_piece_count;i>1;i--){
                    memory_pieces[i]=memory_pieces[i-1];
                    mem_set_state(i,mem_get_state(i-1));
                }
                memory_pieces[pos+1]=memory_pieces[pos]+length;
                memory_piece_count++;
            }
        }else if(!eleje_annyi && vege_annyi){
            #if (MEM_DEBUG==1)
                print("!e&v");
            #endif
            memory_pieces[pos+1]-=length;
        }else{
            #if (MEM_DEBUG==1)
                print("!e&!v");
            #endif
            for(uint8 i=memory_piece_count-1;i>pos;i--){
                memory_pieces[i+2]=memory_pieces[i];
            }
            memory_piece_count+=2;
            mem_set_state(memory_piece_count-1,mem_get_state(memory_piece_count-3));
            mem_set_state(memory_piece_count-2,mem_get_state(memory_piece_count-4));
            memory_pieces[pos+1]=(uint32)address;
            memory_pieces[pos+2]=(uint32)address+length;
            mem_set_state(pos+1,MEM_STATE_FREE);
            mem_set_state(pos+2,MEM_STATE_RESV);
        }
        #if (MEM_DEBUG==1)
            nextLine();
        #endif
    }
    #if (MEM_DEBUG==1)
    else{
        print("Memory error: Free; Can not find the desired location.");
        nextLine();
    }
    #endif
}

void* malloc(uint32 length){

    #if MEM_DEBUG==1
        print("Allocated ");
        printint(length);
        print(" bytes at 0x");
    #endif

    uint8 pos=0;
    uint8 can_resv=0;

    for(uint8 i=(mem_get_state(0) == MEM_STATE_FREE ? 0 : 1);i<memory_piece_count;i+=2){
        if((i==memory_piece_count-1 ? heap_size : memory_pieces[i+1])-memory_pieces[i]>=length){
            pos=i;
            can_resv=1;
            break;
        }
    }

    #if MEM_DEBUG==1
        printhex(memory_pieces[pos]+MEM_BASE);
        print(" ");
    #endif

    void* retval;
    if(can_resv){
        if((pos==memory_piece_count-1 ? heap_size : memory_pieces[pos+1])-memory_pieces[pos]==length){
            if(pos==0){
                retval=(void*)memory_pieces[0];
                for(uint8 i=1;i<memory_piece_count-1;i++){
                    memory_pieces[i]=memory_pieces[i+1];
                    mem_set_state(i,mem_get_state(i+1));
                }
                mem_set_state(0,MEM_STATE_RESV);
                memory_pieces[0]=0;
                #if (MEM_DEBUG==1)
                    print("p0");
                #endif
            }
            else{
                retval=(void*)memory_pieces[pos];
                for(uint8 i=pos-1;i<length-2;i++){
                    memory_pieces[i]=memory_pieces[i+2];
                }
                memory_piece_count--;
                #if (MEM_DEBUG==1)
                    print("pn0");
                #endif
            }
        }else{
            if(pos==0){
                retval=(void*)memory_pieces[0];
                for(uint8 i=memory_piece_count-1;i>0;i++){
                    memory_pieces[i+1]=memory_pieces[i];
                    mem_set_state(i+1,mem_get_state(i));
                }
                memory_piece_count++;
                mem_set_state(0,MEM_STATE_RESV);
                memory_pieces[1]=memory_pieces[0]+length;
                mem_set_state(1,MEM_STATE_FREE);
                #if (MEM_DEBUG==1)
                    print("np0");
                #endif
            }else{
                retval=(void*)memory_pieces[pos];
                memory_pieces[pos]+=length;
                #if (MEM_DEBUG==1)
                    print("npn0");
                #endif
            }
        }
    }

    #if MEM_DEBUG==1
        nextLine();
    #endif

    return (void*)((uint32)retval + MEM_BASE);

    /*print("Allocated memory from 0x");
    printhex(return_val);
    print(" to 0x");
    printhex(return_val+length-1);
    print(", ");
    printint(length);
    print(" bytes");
    nextLine();*/
}

/*void mem_clear_overleaps(){
    uint8 q=1;
    while(q){
        q=0;
        for(uint8 i=0;i<memory_piece_count-1;i++){
            if(memory_pieces[i]==memory_pieces[i+1]){
                print("address ");
                memory_pieces[i]=memory_pieces[i+1];
                mem_set_state(i,mem_get_state(i+1));
                for(uint8 j=i+1;j<memory_piece_count;j++){
                    memory_pieces[j]=memory_pieces[j+1];
                    mem_set_state(j,mem_get_state(j+1));
                }
                memory_piece_count--;
                q=1;
                break;

            }
            if(mem_get_state(i)==mem_get_state(i+1)){
                print("state ");
                for(uint8 j=i;j<memory_piece_count;j++){
                    memory_pieces[j]=memory_pieces[j+1];
                    mem_set_state(j,mem_get_state(j+1));
                }
                memory_piece_count--;
                q=1;
                break;
            }
        }
    }
}*/

void memory_init(){
    uint8 q=1;
    while(q){
        q=0;
        for(uint8 i=0;i<memory_map_region_count-1;i++){
            if((uint32)memroy_map_region_base[i]>(uint32)memroy_map_region_base[i+1]){
                uint32 c = memroy_map_region_base[i];
                memroy_map_region_base[i]=memroy_map_region_base[i+1];
                memroy_map_region_base[i+1]=c;

                c=memory_map_region_length[i];
                memory_map_region_length[i]=memory_map_region_length[i+1];
                memory_map_region_length[i+1]=c;

                c=memory_map_region_type[i];
                memory_map_region_type[i]=memory_map_region_type[i+1];
                memory_map_region_type[i+1]=c;

                q=1;
            }
        }
    }

    uint8 elso=1;

    for(uint32 i=memory_map_region_count;i>0;i--){
        if(memroy_map_region_base[i]>=0x100000)
        if(elso){
            if(memory_map_region_type[i]==1){
                heap_size=memroy_map_region_base[i]+memory_map_region_length[i]-0x100000;
                elso=0;
            }else{
                /* allocate from memory_map_region_base[i] to memory_map_region_base[i]+memory_map_region_length[i] */
            }
        }
    }

    memory_pieces[0]=0;
    mem_set_state(0,MEM_STATE_FREE);
    memory_piece_count=1;
}

void memcpy(uint8* from, uint8* to, uint32 amount){
    for(uint32 i=0;i<amount;i++){
        to[i]=from[i];
    }
}

uint8 memcmp(uint8* m1, uint8* m2, uint32 length){
    for(uint32 i=0;i<length;i++){
        if(m1[i]!=m2[i]){
            return 0;
        }
    }
    return 1;
}

void mmap_terminal_command(string_array* args){
    print_memory_map();
    return;
}
void memory_manager_terminal_command(string_array* args){
    if(args->length==0){
        printMemoryManagerTable();
    }
}
void memset(uint8* address, uint32 length, uint8 byte){
    for(uint32 i=0;i<length;i++){
        address[i]=byte;
    }
    return;
}